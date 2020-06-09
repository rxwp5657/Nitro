#include "../../include/graphics/framebuffer.hpp"

namespace nitro
{
    namespace graphics
    {
        Framebuffer::Framebuffer()
        :   framebuffer_{0},
            width_{0},
            height_{0}
        {
            glGenFramebuffers(1, &framebuffer_);
        }

        Framebuffer::Framebuffer(int width, int height)
        :   framebuffer_{0},
            width_{width},
            height_{height}
        {
            glGenFramebuffers(1, &framebuffer_);
        }

        void Framebuffer::AttachTexture(const Texture& texture, GLenum attachment_t) const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

            if(texture.Type() == GL_TEXTURE_2D)
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_t, GL_TEXTURE_2D, texture.TextureReference(), 0);
            
            else if(texture.Type() == GL_TEXTURE_CUBE_MAP)
                glFramebufferTexture(GL_FRAMEBUFFER,  attachment_t, texture.TextureReference(), 0);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        bool Framebuffer::Complete() const
        {
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }

        void Framebuffer::Bind() const
        {
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Framebuffer is not complete \n");
            
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glViewport(0, 0, width_, height_);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        }

        void Framebuffer::Unbind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Framebuffer::Size(int width, int height)
        {
            width_  = width;
            height_ = height;
        }

        void Framebuffer::DeleteColorBuffers() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
}