#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>

#include "./stb_image.hpp"
#include "./drawable.hpp"
#include "./shader.hpp"

namespace nitro
{
    namespace graphics
    {
        class Texture : public Drawable
        {
        public:

            Texture();

            Texture(const Texture& texture);

            Texture(Texture&& texture) noexcept;

            Texture(const std::string& file_name, 
                    const std::string& dir,
                    const std::string& name = "texture_diffuse");

            Texture(const std::string& dir,
                    const std::vector<std::string>& texture_names,
                    const std::string& name = "texture_diffuse");

            Texture(const std::string& name,
                    int width, 
                    int height, 
                    GLenum target, 
                    GLenum internal_format, 
                    GLenum format, 
                    GLenum type);

            ~Texture();

            Texture& operator = (const Texture&  texture);
            Texture& operator = (Texture&& texture) noexcept;
            
            std::string get_path()    const;
            std::string Name()        const;

            GLuint TextureReference() const;

            void TextureUnit(GLint texture_unit, int uniform_location, int texture_number);
            
            void PostProcessing(const Shader& shader) const;

            void Erase() override;
            void Draw(const Shader& shader, bool default_framebuffer = true) override;

        private:
            GLuint texture_;
            GLint  texture_unit_;
            GLuint type_;
            std::string name_;
            std::string path_;
            int uniform_location_;
            bool has_num_;
            GLuint quad_vba_;
            GLuint quad_vbo_;
            std::vector<float> quad_vertices_;
            
            void Setup(const Shader& shader) override;
            void InitQuad();
        };
    }
}

#endif