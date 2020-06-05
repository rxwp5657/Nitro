#ifndef SHADOWS_H
#define SHADOWS_H

#include <vector>

#include "../graphics/shader.hpp"
#include "../graphics/texture.hpp"
#include "../graphics/framebuffer.hpp"
#include "./actor.hpp"

namespace nitro
{
    namespace core
    {
        class Shadow
        {
        public:

            Shadow(int num_shadow_maps, int pcf);
            ~Shadow() = default;
            
            bool CastShadow() const;
            void CastShadow(bool cast);

            unsigned int ShadowMap(int index) const;
            graphics::Texture ShadowTexture(int index) const;

            virtual void SetupShadows() = 0;
            virtual void PostProcess(const std::map<std::string, graphics::Shader>& shaders) = 0;
            virtual void DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                     const std::vector<std::shared_ptr<Actor>>& actors) = 0;

        protected:
            bool cast_shadows_;
            bool set_up_;
            int  pcf_;
            std::vector<graphics::Framebuffer> framebuffers_;
            std::vector<graphics::Texture> shadow_maps_;
        };
    }
}

#endif