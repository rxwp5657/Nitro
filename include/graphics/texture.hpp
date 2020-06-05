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
        
        struct TextureFile
        {
            int width;
            int height;
            unsigned int format;
            unsigned char *data;
        };

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
                    unsigned int target, 
                    unsigned int internal_format, 
                    unsigned int format, 
                    unsigned int data_type,
                    unsigned int filter);

            ~Texture();

            Texture& operator = (const Texture&  texture);
            Texture& operator = (Texture&& texture) noexcept;
            
            int          Width()            const;
            int          Height()           const;
            int          Unit()             const;
            std::string  Path()             const;
            std::string  Name()             const;
            unsigned int TextureReference() const;
            unsigned int Type()             const;
            unsigned int InternalFormat()   const;
            unsigned int Format()           const;
            unsigned int DataType()         const;

            void TextureUnit(GLint texture_unit, int uniform_location, int texture_number);
            void TextureUnit(GLint texture_unit, int uniform_location, const std::string& name);
                        
            void DrawQuad() const;

            void Erase() override;
            void Draw(const Shader& shader, bool default_framebuffer = true) override;
            
        private:
            unsigned int texture_;
            unsigned int type_;
            unsigned int internal_format_;
            unsigned int format_;
            unsigned int data_type_;
            
            unsigned int texture_unit_;
            int  uniform_location_;
            int  widht_;
            int  height_;
            bool has_num_;

            std::string name_;
            std::string path_;
            
            unsigned int quad_vba_;
            unsigned int quad_vbo_;
            std::vector<float> quad_vertices_;
            
            void Setup(const Shader& shader) override;
            void InitQuad();

            TextureFile LoadTextureFile(const std::string& path);
            
            void Create2DTexture(int width, int height, unsigned int internal_format, unsigned int format, unsigned int data_type, unsigned char *data, unsigned int filter, bool mipmap);          
            void CreateCubeMap(int width, int height, unsigned int internal_format, unsigned int format, unsigned int data_type);
            void CreateCubeMap(const std::vector<TextureFile>& file_data, unsigned int internal_format, unsigned int data_type ,unsigned int filter);
        };
    }
}

#endif