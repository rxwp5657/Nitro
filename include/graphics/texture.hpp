#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>
#include <drawable.hpp>
#include <shader.hpp>

namespace nitro
{
    namespace graphics
    {
        class Texture : public Drawable
        {
        public:
            Texture(const std::string& file_name, 
                    const std::string& dir, 
                    const GLint texture_unit = GL_TEXTURE0,
                    const std::string& name = "albedo",
                    const int uniform_location = 0);
            
            Texture(const std::string& file_name, 
                    const std::string& dir,
                    const std::string& name = "albedo");
            
            ~Texture();

            std::string get_path() const;
            
            void TextureUnit(GLint texture_unit, 
                             int uniform_location,
                             int texture_number);

            std::string Name();

            void Erase() override;
            void Draw(const Shader& shader) override;
        private:
            GLuint texture_;
            GLint texture_unit_;
            std::string name_;
            std::string path_;
            int uniform_location_;
            void Setup(const Shader& shader) override;
        };
    }
}

#endif