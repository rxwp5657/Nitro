#include <texture.hpp>

namespace nitro
{
    namespace graphics
    {
        Texture::Texture()
        :   texture_{0},
            texture_unit_{0},
            type_{0},
            name_{""},
            path_{""},
            uniform_location_{0},
            has_num_{false},
            quad_vba_{0},
            quad_vbo_{0},
            quad_vertices_{
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f}
        {
            InitQuad();
        }

        Texture::Texture(const Texture& texture)
        :   texture_{texture.texture_},
            texture_unit_{texture.texture_unit_},
            type_{texture.type_},
            name_{texture.name_},
            path_{texture.path_},
            uniform_location_{texture.uniform_location_},
            has_num_{texture.has_num_},
            quad_vba_{texture.quad_vba_},
            quad_vbo_{texture.quad_vbo_},
            quad_vertices_{texture.quad_vertices_}
        {
            
        }

        Texture::Texture(Texture&& texture) noexcept
        :   texture_{texture.texture_},
            texture_unit_{texture.texture_unit_},
            type_{texture.type_},
            name_{texture.name_},
            path_{texture.path_},
            uniform_location_{texture.uniform_location_},
            has_num_{texture.has_num_},
            quad_vba_{texture.quad_vba_},
            quad_vbo_{texture.quad_vbo_},
            quad_vertices_{std::move(texture.quad_vertices_)}
        {
            texture.texture_          = 0;
            texture.texture_unit_     = 0;
            texture.type_             = 0;
            texture.name_             = "";
            texture.path_             = "";
            texture.uniform_location_ = 0;
            texture.has_num_          = false;
            texture.quad_vba_         = 0;
            texture.quad_vbo_         = 0;
        }

        Texture::Texture(const std::string& file_name, 
                         const std::string& dir,
                         const std::string& name)
        :   type_{GL_TEXTURE_2D},
            name_{name},
            path_{file_name},
            has_num_{false},
            quad_vba_{0},
            quad_vbo_{0},
            quad_vertices_{
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f}
        {
            std::string path = dir + '/' + file_name;

            glGenTextures(1, &texture_);

            int width, height, nr_channels;
            stbi_set_flip_vertically_on_load(false);

            InitQuad();

            unsigned char *data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
            if (data)
            {
                GLenum format = GL_RGB;
                if (nr_channels == 1)
                    format = GL_RED;
                else if (nr_channels == 3)
                    format = GL_RGB;
                else if (nr_channels == 4)
                    format = GL_RGBA;
                
                glBindTexture(GL_TEXTURE_2D, texture_);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else
            {
                throw std::runtime_error("Unable to load texture at path: " + path);
            }
            stbi_image_free(data);
        }

        Texture::Texture(const std::string& dir,
                         const std::vector<std::string>& texture_names,
                         const std::string& name)
        :   type_{GL_TEXTURE_CUBE_MAP},
            name_{name},
            path_{dir},
            has_num_{false}
        {
            if(texture_names.size() < 6)
                throw std::runtime_error{"(Texture/CubeMapTexture) Not enough textures given for loading skybox"};
            
            glGenTextures(1, &texture_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
            
            int width, height, nr_channels;
            stbi_set_flip_vertically_on_load(false);

            std::string path = "../resources/cubemaps/" + dir + '/';
            std::string file_path{""};

            unsigned char *data;
            for(int i = 0; i < texture_names.size(); i++)
            {
                file_path = path + texture_names[i];
                data  = stbi_load(file_path.c_str(), &width, &height, &nr_channels, 0);
                if (data)
                {
                    GLenum format = GL_RGB;
                    if (nr_channels == 1)
                        format = GL_RED;
                    else if (nr_channels == 3)
                        format = GL_RGB;
                    else if (nr_channels == 4)
                        format = GL_RGBA;
                    
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }
                else
                {
                    throw std::runtime_error{("(Texture/CubeMapTexture) Unable to load texture: " + file_path).c_str()};
                }    
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        }

        Texture::Texture(const std::string& name,
                         int width, 
                         int height, 
                         GLenum target, 
                         GLenum internal_format, 
                         GLenum format, 
                         GLenum type)
        :   type_{target},
            name_{name},
            path_{""},
            has_num_{true}
        {
            switch (target)
            {
            case GL_TEXTURE_2D:
                
                InitQuad();
                glGenTextures(1, &texture_);
                glBindTexture(GL_TEXTURE_2D, texture_);
                glTexImage2D(GL_TEXTURE_2D, 0,internal_format, width, height, 0, format, type, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                break;
            
            case GL_TEXTURE_CUBE_MAP:
                
                glGenTextures(1, &texture_);
                glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);

                for (unsigned int i = 0; i < 6; ++i)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, format, type, NULL);
                
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                break;

            default:
                break;
            }
        }

        Texture::~Texture()
        {
            
        }

        Texture& Texture::operator = (const Texture&  texture)
        {
            if(this == &texture) return *this;

            texture_ =  texture.texture_;
            texture_unit_ = texture.texture_unit_;
            type_ = texture.type_;
            name_ = texture.name_;
            path_ = texture.path_;
            uniform_location_ = texture.uniform_location_;
            has_num_          = texture.has_num_;
            quad_vba_         = texture.quad_vba_;
            quad_vbo_         = texture.quad_vbo_;
            quad_vertices_    = texture.quad_vertices_;

            return *this;
        }
        
        Texture& Texture::operator = (Texture&& texture) noexcept
        {
            if(this == &texture) return *this;

            texture_ = texture.texture_;
            texture_unit_ = texture.texture_unit_;
            type_ = texture.type_;
            name_ = texture.name_;
            path_ = texture.path_;
            uniform_location_ = texture.uniform_location_;
            has_num_          = texture.has_num_;
            quad_vba_         = texture.quad_vba_;
            quad_vbo_         = texture.quad_vbo_;
            quad_vertices_    = std::move(texture.quad_vertices_);

            texture.texture_          = 0;
            texture.texture_unit_     = 0;
            texture.type_             = 0;
            texture.name_             = "";
            texture.path_             = "";
            texture.uniform_location_ = 0;
            texture.has_num_          = false;
            texture.quad_vba_         = 0;
            texture.quad_vbo_         = 0;

            return *this;
        }

        std::string Texture::get_path() const
        {
            return path_;
        }

        std::string Texture::Name() const
        {
            return name_;
        }

        GLuint Texture::TextureReference() const
        {
            return texture_;
        }

        void Texture::TextureUnit(GLint texture_unit, 
                                  int uniform_location,
                                  int texture_number)
        {
            texture_unit_     = texture_unit;
            uniform_location_ = uniform_location;
            if(!has_num_)
            {
                name_ += std::to_string(texture_number);
                has_num_ = true;
            }
        }

        void Texture::InitQuad()
        {
            glGenVertexArrays(1, &quad_vba_);
            glGenBuffers(1, &quad_vbo_);

            glBindVertexArray(quad_vba_);
            glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
            glBufferData(GL_ARRAY_BUFFER, quad_vertices_.size(), &quad_vertices_[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Texture::Erase()
        {
            glDeleteTextures(1,&texture_);
        }

        void Texture::Setup(const Shader& shader)
        {

        }

        void Texture::PostProcessing(const Shader& shader) const
        {
            shader.Use();
            glBindVertexArray(quad_vba_);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }

        void Texture::Draw(const Shader& shader, bool default_framebuffer)
        {
            glActiveTexture(texture_unit_);
            shader.SetUniformInt(name_.c_str(), uniform_location_);
            glBindTexture(type_, texture_);
        }
    }
}