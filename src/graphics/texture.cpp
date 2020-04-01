#include <texture.hpp>

namespace nitro
{
    namespace graphics
    {
        Texture::Texture(const std::string& file_name, 
                         const std::string& dir,
                         const std::string& name)
        : type_{GL_TEXTURE_2D},
          name_{name},
          path_{file_name}
        {
            std::string path = dir + '/' + file_name;

            glGenTextures(1, &texture_);

            int width, height, nr_channels;
            stbi_set_flip_vertically_on_load(false);

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
        : type_{GL_TEXTURE_CUBE_MAP},
          name_{name},
          path_{dir}
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

        Texture::~Texture()
        {
            
        }

        std::string Texture::get_path() const
        {
            return path_;
        }

        void Texture::TextureUnit(GLint texture_unit, 
                                  int uniform_location,
                                  int texture_number)
        {
            texture_unit_     = texture_unit;
            uniform_location_ = uniform_location;
            name_ += std::to_string(texture_number);
        }

        std::string Texture::Name()
        {
            return name_;
        }

        void Texture::Erase()
        {
            glDeleteTextures(1,&texture_);
        }

        void Texture::Setup(const Shader& shader)
        {

        }

        void Texture::Draw(const Shader& shader)
        {
            glActiveTexture(texture_unit_);
            shader.SetUniformInt(name_.c_str(), uniform_location_);
            glBindTexture(type_, texture_);
        }
    }
}