#ifndef POST_PROCESS
#define POST_PROCESS

#include "./shader.hpp"
#include "./texture.hpp"

namespace nitro
{
    namespace graphics
    {
        struct PostProcess
        {
            virtual ~PostProcess()  = default;
            virtual void Process(const Shader& shader, Texture& in, Texture& out) = 0;
        };
    }
}

#endif