#include <texture.hpp>

namespace nitro
{
    namespace graphics
    {
        Texture::Texture(const std::string& file_name, 
                         const std::string& dir, 
                         const GLint texture_unit,
                         const std::string& name,
                         const int uniform_location)
        : texture_unit_{texture_unit},
          name_{name},
          path_{file_name},
          uniform_location_{uniform_location}
        {
            std::string path = dir + '/' + file_name;

            glGenTextures(1, &texture_);
            glBindTexture(GL_TEXTURE_2D, texture_); 

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int width, height, nr_channels;
            stbi_set_flip_vertically_on_load(true);

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

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                throw std::runtime_error("Unable to load texture at path: " + path);
                stbi_image_free(data);
            }
        }

        std::string Texture::get_path() const
        {
            return path_;
        }

        Texture::~Texture()
        {
            Erase();
        }

        void Texture::Erase()
        {
            //glDeleteTextures(1,&texture_);
        }

        void Texture::Setup(const Shader& shader)
        {

        }

        void Texture::Draw(const Shader& shader)
        {
            shader.SetUniformInt(name_.c_str(),uniform_location_);
            glActiveTexture(texture_unit_);
            glBindTexture(GL_TEXTURE_2D, texture_);
        }
    }
}