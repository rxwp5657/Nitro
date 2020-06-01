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
            Framebuffer(unsigned int viewport_width, 
                        unsigned int viewport_height, 
                        bool color   = false, 
                        bool depht   = false,
                        bool stencil = false);
            
            void AttachTexture(const GLenum type, const GLuint texture, bool is_cubemap = false) const;
            void AttachRenderBuffer(const GLenum type, const GLuint) const;
            void WindowViewPort(const bool is_window) const;
            void Bind() const;
            void Unbind() const;
            GLuint GenRenderBuffer(GLenum format); 

        private:  
            unsigned int viewport_width_;
            unsigned int viewport_height_;
            bool   is_window_;
            GLuint framebuffer_;
            GLuint depht_render_buffer_;
            GLuint color_render_buffer_;
            GLuint stencil_render_buffer_;
        };
    }
}

#endif