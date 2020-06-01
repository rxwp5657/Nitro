#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "./shader.hpp"

namespace nitro
{
    namespace graphics
    {
        struct Drawable
        {
        public:
            virtual ~Drawable()  = default;
            virtual void Erase() = 0;
            virtual void Setup(const Shader& shader) = 0;
            virtual void Draw(const  Shader& shader, bool default_framebuffer = true) = 0;
        };
    }
}

#endif


