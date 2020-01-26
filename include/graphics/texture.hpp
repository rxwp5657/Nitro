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
                    const GLenum texture_unit = GL_TEXTURE0,
                    const std::string& name = "albedo",
                    const int uniform_location = 0);
            
            ~Texture();

            void Erase() override;
            void Draw(const Shader& shader) override;
        private:
            GLuint texture_;
            GLenum texture_unit_;
            std::string name_;
            int uniform_location_;
            void Setup(const Shader& shader) override;
        };
    }
}

#endif