#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./texture.hpp"

namespace nitro
{
    namespace graphics
    {
        class Framebuffer
        {
        public:
            Framebuffer();
            Framebuffer(int width, int height);

            void AttachTexture(const Texture& texture, GLenum attachment_t);
            //void AttachRenderBuffer(const RenderBuffer& renderBuffer);

            bool Complete();

            void Bind();
            void Unbind();
            
            void Size(int width, int height);
            void DeleteColorBuffers();
            
        private:
            unsigned int framebuffer_;
            int width_;
            int height_;
        };
    }
}

#endif