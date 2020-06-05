#include "../../include/graphics/texture.hpp"

namespace nitro
{
    namespace graphics
    {
        Texture::Texture()
        :   texture_{0},
            type_{0},
            internal_format_{0},
            format_{0},
            data_type_{0},
            texture_unit_{0},
            uniform_location_{0},
            widht_{0},
            height_{0},
            has_num_{false},
            name_{""},
            path_{""},
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
            type_{texture.type_},
            internal_format_{texture.internal_format_},
            format_{texture.format_},
            data_type_{texture.data_type_},
            texture_unit_{texture.texture_unit_},
            uniform_location_{texture.uniform_location_},
            widht_{texture.widht_},
            height_{texture.height_},
            has_num_{texture.has_num_},
            name_{texture.name_},
            path_{texture.path_},
            quad_vba_{texture.quad_vba_},
            quad_vbo_{texture.quad_vbo_},
            quad_vertices_{texture.quad_vertices_}
        {
            
        }

        Texture& Texture::operator = (const Texture&  texture)
        {
            if(this == &texture) return *this;

            texture_          = texture.texture_;
            type_             = texture.type_;
            internal_format_  = texture.internal_format_;
            format_           = texture.format_;
            data_type_        = texture.data_type_;
            texture_unit_     = texture.texture_unit_;
            uniform_location_ = texture.uniform_location_;
            widht_            = texture.widht_;
            height_           = texture.height_;
            has_num_          = texture.has_num_;
            name_             = texture.name_;
            path_             = texture.path_;
            quad_vba_         = texture.quad_vba_;
            quad_vbo_         = texture.quad_vbo_;
            quad_vertices_    = texture.quad_vertices_;

            return *this;
        }

        Texture::Texture(Texture&& texture) noexcept
        :   texture_{texture.texture_},
            type_{texture.type_},
            internal_format_{texture.internal_format_},
            format_{texture.format_},
            data_type_{texture.data_type_},
            texture_unit_{texture.texture_unit_},
            uniform_location_{texture.uniform_location_},
            widht_{texture.widht_},
            height_{texture.height_},
            has_num_{texture.has_num_},
            name_{texture.name_},
            path_{texture.path_},
            quad_vba_{texture.quad_vba_},
            quad_vbo_{texture.quad_vbo_},
            quad_vertices_{std::move(texture.quad_vertices_)}
        {
            texture.texture_          = 0;
            texture.type_             = 0;
            texture.internal_format_  = 0;
            texture.format_           = 0; 
            texture.data_type_        = 0; 
            texture.texture_unit_     = 0;
            texture.uniform_location_ = 0; 
            texture.has_num_          = false; 
            texture.widht_            = 0;
            texture.height_           = 0; 
            texture.name_             = ""; 
            texture.path_             = ""; 
            texture.quad_vba_         = 0; 
            texture.quad_vbo_         = 0; 
        }

        Texture& Texture::operator = (Texture&& texture) noexcept
        {
            if(this == &texture) return *this;

            texture_          = texture.texture_;
            type_             = texture.type_;
            internal_format_  = texture.internal_format_;
            format_           = texture.format_;
            data_type_        = texture.data_type_;
            texture_unit_     = texture.texture_unit_;
            uniform_location_ = texture.uniform_location_;
            widht_            = texture.widht_;
            height_           = texture.height_;
            has_num_          = texture.has_num_;
            name_             = texture.name_;
            path_             = texture.path_;
            quad_vba_         = texture.quad_vba_;
            quad_vbo_         = texture.quad_vbo_;
            quad_vertices_    = texture.quad_vertices_;

            texture.texture_          = 0;
            texture.type_             = 0;
            texture.internal_format_  = 0;
            texture.format_           = 0; 
            texture.data_type_        = 0; 
            texture.texture_unit_     = 0;
            texture.uniform_location_ = 0; 
            texture.widht_            = 0;
            texture.height_           = 0;
            texture.has_num_          = false;  
            texture.name_             = ""; 
            texture.path_             = ""; 
            texture.quad_vba_         = 0; 
            texture.quad_vbo_         = 0; 

            return *this;
        }

        Texture::Texture(const std::string& file_name, const std::string& dir, const std::string& name)
        :   type_{GL_TEXTURE_2D},
            internal_format_{0},
            format_{0},
            data_type_{GL_UNSIGNED_BYTE},
            texture_unit_{0},
            uniform_location_{0},
            widht_{0},
            height_{0},
            has_num_{false},
            name_{name},
            path_{file_name},
            quad_vba_{0},
            quad_vbo_{0},
            quad_vertices_{
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f}
        {
            std::string path = dir + '/' + file_name;
            TextureFile file_data = LoadTextureFile(path);
            
            InitQuad();

            Create2DTexture(file_data.width, 
                            file_data.height, 
                            file_data.format, 
                            file_data.format, 
                            GL_UNSIGNED_BYTE, 
                            file_data.data, GL_LINEAR, true);

            internal_format_ = file_data.format;
            format_          = file_data.format;
            widht_           = file_data.width;
            height_          = file_data.height;

            stbi_image_free(file_data.data);
        }

        Texture::Texture(const std::string& dir, const std::vector<std::string>& texture_names, const std::string& name)
        :   type_{GL_TEXTURE_CUBE_MAP},
            internal_format_{0},
            format_{0},
            data_type_{GL_UNSIGNED_BYTE},
            texture_unit_{0},
            uniform_location_{0},
            has_num_{false},
            name_{name},
            path_{dir},
            quad_vba_{0},
            quad_vbo_{0},
            quad_vertices_{}
        {
            if(texture_names.size() < 6)
                throw std::runtime_error{"(Texture/CubeMapTexture) Not enough textures given for loading skybox"};

            std::vector<TextureFile> file_data{};

            std::string path = "../resources/cubemaps/" + dir + '/';

            for(const auto file_name : texture_names)
                file_data.push_back(LoadTextureFile(path + file_name));
            
            CreateCubeMap(file_data, file_data[0].format, GL_UNSIGNED_BYTE, GL_LINEAR);
            
            internal_format_ = file_data[0].format;
            format_          = file_data[0].format;
            widht_           = file_data[0].width;
            height_          = file_data[0].height;

            for(const auto f_data : file_data) stbi_image_free(f_data.data);
        }

        Texture::Texture(const std::string& name,
                         int width, 
                         int height, 
                         unsigned int target, 
                         unsigned int internal_format, 
                         unsigned int format, 
                         unsigned int data_type,
                         unsigned int filter)
        :   texture_{0},
            type_{target},
            internal_format_{internal_format},
            format_{format},
            data_type_{data_type},
            texture_unit_{0},
            uniform_location_{0},
            widht_{width},
            height_{height},
            has_num_{true},
            name_{name},
            path_{""},
            quad_vba_{0},
            quad_vbo_{0},
            quad_vertices_{
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f}
        {
            switch (target)
            {
            case GL_TEXTURE_2D:
                InitQuad();
                Create2DTexture(width, height, internal_format, format, data_type, NULL, filter, false);                
                break;
            
            case GL_TEXTURE_CUBE_MAP:
                CreateCubeMap(width, height, internal_format, format, data_type);
                //CreateCubeMap({TextureFile{widht_, height_, format, NULL}}, internal_format , type, GL_NEAREST);
                break;
            default:
                break;
            }
        }

        Texture::~Texture()
        {
            
        }

        int Texture::Width()  const
        {
            return widht_;
        }

        int Texture::Height() const
        {
            return height_;
        }

        int Texture::Unit() const
        {
            return uniform_location_;
        }

        std::string Texture::Path() const
        {
            return path_;
        }

        std::string Texture::Name() const
        {
            return name_;
        }

        unsigned int Texture::TextureReference() const
        {
            return texture_;
        }

        unsigned int Texture::Type() const
        {
            return type_;
        }

        unsigned int Texture::InternalFormat() const
        {
            return internal_format_;
        }

        unsigned int Texture::Format() const
        {
            return format_;
        }

        unsigned int Texture::DataType() const
        {
            return data_type_;
        }

        void Texture::Erase()
        {
            glDeleteTextures(1,&texture_);
        }

        void Texture::DrawQuad()const
        {
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

        void Texture::TextureUnit(GLint texture_unit, int uniform_location, int texture_number)
        {
            texture_unit_     = texture_unit;
            uniform_location_ = uniform_location;

            if(!has_num_)
            {
                name_ += std::to_string(texture_number);
                has_num_ = true;
            }
        }

        void Texture::TextureUnit(GLint texture_unit, int uniform_location, const std::string& name)
        {
            texture_unit_     = texture_unit;
            uniform_location_ = uniform_location;
            name_             = name;
        }

        void Texture::InitQuad()
        {
            glGenVertexArrays(1, &quad_vba_);
            glGenBuffers(1, &quad_vbo_);

            glBindVertexArray(quad_vba_);
            glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
            glBufferData(GL_ARRAY_BUFFER, quad_vertices_.size() * sizeof(float), &quad_vertices_[0], GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        TextureFile Texture::LoadTextureFile(const std::string& path)
        {
            int width, height, nr_channels;
            unsigned int format = GL_RGB;

            stbi_set_flip_vertically_on_load(false);

            unsigned char *data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

            if (data)
            {
                switch (nr_channels)
                {
                case 1:
                    format = GL_RED;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                case 4:
                    format = GL_RGBA;
                    break;
                default:
                    format = GL_RGB;
                    break;
                }
            }
            else
            {
                throw std::runtime_error("Unable to load texture at path: " + path);
            }
            return TextureFile{width, height, format, data};
        }

        void Texture::Create2DTexture(int width, 
                                      int height, 
                                      unsigned int internal_format, 
                                      unsigned int format, 
                                      unsigned int data_type, 
                                      unsigned char *data, 
                                      unsigned int filter,
                                      bool mipmap)
        {
            glGenTextures(1, &texture_);

            glBindTexture(GL_TEXTURE_2D, texture_);
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, data);
            
            if(mipmap)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            }
            else
            {   //GL_NEAREST
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindTexture(GL_TEXTURE_2D,0);
        }

        void Texture::CreateCubeMap(const std::vector<TextureFile>& file_data, unsigned int internal_format, unsigned int data_type ,unsigned int filter)
        {
            glGenTextures(1, &texture_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);

            auto size = file_data.size();
            TextureFile f_data{};

            for(int i = 0; i < 6; i++)
            {
                f_data = (size > 1) ? file_data[i] : file_data[0]; 
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, f_data.width, f_data.height, 0, f_data.format, data_type, f_data.data);
            }
            
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        }

        void Texture::CreateCubeMap(int width, int height, unsigned int internal_format, unsigned int format, unsigned int data_type)
        {
            glGenTextures(1, &texture_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);

            for (unsigned int i = 0; i < 6; ++i)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, format, data_type, NULL);
            
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        }

        void Texture::Setup(const Shader& shader)
        {

        }
    }
}