#ifndef GAUSSIAN_BLUR
#define GAUSSIAN_BLUR

#include <string>
#include "../graphics/framebuffer.hpp"
#include "../graphics/post_process.hpp"

namespace nitro
{
    namespace core
    {
        class GaussianBlur : public graphics::PostProcess
        {
        public:

            GaussianBlur(int width, int height);
            void Process(const graphics::Shader& shader, graphics::Texture& in, graphics::Texture& out) override;

        private:
            graphics::Framebuffer framebuffer_;
            void Subprocess(const graphics::Shader& shader, graphics::Texture& in, graphics::Texture& out, bool vertical);
        };
    }
}

#endif