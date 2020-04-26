#include <framebuffer.hpp>

namespace nitro
{
    namespace graphics
    {
        Framebuffer::Framebuffer(
        unsigned int viewport_width, 
        unsigned int viewport_height,
        bool color, 
        bool depht,
        bool stencil)
        : viewport_width_{viewport_width},
          viewport_height_{viewport_height},
          is_window_{false}
        {
            glGenFramebuffers(1, &framebuffer_);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            if(color)
            {
                color_render_buffer_ =  GenRenderBuffer(GL_RGBA4);
                AttachRenderBuffer(GL_COLOR_ATTACHMENT0, color_render_buffer_);
            }
            else if(depht)
            {
                depht_render_buffer_ = GenRenderBuffer(GL_DEPTH_COMPONENT16);
                AttachRenderBuffer(GL_DEPTH_ATTACHMENT, depht_render_buffer_);
            }
            else if(stencil)
            {
                stencil_render_buffer_ = GenRenderBuffer(GL_STENCIL_INDEX8);
                AttachRenderBuffer(GL_STENCIL_ATTACHMENT, stencil_render_buffer_);
            }
            else
            {
                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
            }
               
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        Framebuffer::Framebuffer()
        : viewport_width_{1024},
          viewport_height_{1024},
          is_window_{false}
        {
            glGenFramebuffers(1, &framebuffer_);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Framebuffer::AttachTexture(const GLenum type, const GLuint texture, bool is_cubemap) const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

            if(is_cubemap)
                glFramebufferTexture(GL_FRAMEBUFFER, type, texture, 0);

            else
                glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture, 0);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
                  
        }

        void Framebuffer::AttachRenderBuffer(const GLenum type, const GLuint render_buffer) const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, type, GL_RENDERBUFFER, render_buffer);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Framebuffer::WindowViewPort(const bool is_window) const
        {

        }

        void Framebuffer::Bind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                throw std::runtime_error("Unable to bind frame buffer, it isn't complete");
                glBindFramebuffer(GL_FRAMEBUFFER, 0); 
            } 
        }

        void Framebuffer::Unbind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);  
        }

        GLuint Framebuffer::GenRenderBuffer(GLenum format)
        {
            GLuint render_buffer;
            glGenRenderbuffers(1, &render_buffer);
            glBindRenderbuffer(GL_RENDERBUFFER, render_buffer); 
            glRenderbufferStorage(GL_RENDERBUFFER, format, viewport_width_, viewport_height_);  
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            
            return render_buffer; 
        }
    }
}