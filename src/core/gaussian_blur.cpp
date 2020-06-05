#include "../../include/core/gaussian_blur.hpp"

namespace nitro
{
    namespace core
    {
        GaussianBlur::GaussianBlur(int width, int height)
        :   framebuffer_{width, height}
        {

        }

        void GaussianBlur::Process(const graphics::Shader& shader, graphics::Texture& in, graphics::Texture& out)
        {
            Subprocess(shader, in, out, true);
            Subprocess(shader, out, in, false);
        }

        void GaussianBlur::Subprocess(const graphics::Shader& shader, graphics::Texture& in, graphics::Texture& out, bool vertical)
        {
            framebuffer_.AttachTexture(out, GL_COLOR_ATTACHMENT0);
            framebuffer_.Bind();

            shader.Use();
            shader.SetUniformInt("vertical", vertical);
            
            in.TextureUnit(GL_TEXTURE0, 0, "image");
            in.Draw(shader);
            in.DrawQuad();
            
            shader.Disable();
            framebuffer_.Unbind();
        }
    }
}
