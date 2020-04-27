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
            LightShadow(bool shadows, const graphics::Texture& shadow_map) 
            : shadows_{shadows}, shadow_map_{shadow_map} {}
            
            bool Shadow() { return shadows_; }
            void Shadow(bool casts) { shadows_ = casts; }

            virtual ~LightShadow()  = default;
            virtual void DrawShadows(const graphics::Shader& shader,
                                     const graphics::Framebuffer& buffer) = 0;
        
        protected:
            bool shadows_;
            graphics::Texture shadow_map_;
        };
        
    }
}

#endif