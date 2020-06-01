#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "../graphics/shader.hpp"

namespace nitro
{
    namespace utils
    {
        class Debugger
        {
        public:
            Debugger();

            void DebugFrameBuffer(const unsigned int frame_buffer, const graphics::Shader& shader);

        private:
            unsigned int quad_vao_;
            unsigned int quad_vbo_;

            void RenderQuad();
        };    
    }
}


#endif