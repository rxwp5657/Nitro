#ifndef LIGHT_SHADOW_H
#define LIGHT_SHADOW_H

#include <shader.hpp>
#include <framebuffer.hpp>
#include <texture.hpp>

namespace nitro
{
    namespace core
    {
        struct LightShadow
        {
        public:
            LightShadow(bool shadows, bool shadow_setup)
            : shadows_{shadows},  shadow_setup_{shadow_setup} {}
            
            bool Shadow() { return shadows_; }
            void Shadow(bool casts) { shadows_ = casts; }
            
            bool ShadowSetup()  { return shadow_setup_;}
            void ShadowSetup(bool setup_status) { shadow_setup_ = setup_status;}

            GLuint ShadowMap()   {return shadow_map_;} 
            GLuint FrameBuffer() {return framebuffer_;}

            virtual ~LightShadow()  = default;
            virtual void SetupShadows() = 0;
            virtual void DrawShadows(const graphics::Shader& shader) = 0;
            
        protected:
            bool   shadows_;
            bool   shadow_setup_;
            GLuint shadow_map_;
            GLuint framebuffer_;
        };
        
    }
}

#endif