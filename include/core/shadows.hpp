#ifndef SHADOWS_H
#define SHADOWS_H

#include <vector>
#include <shader.hpp>
#include <texture.hpp>
#include <actor.hpp>

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
            virtual void DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                     const std::vector<std::shared_ptr<Actor>>& actors) = 0;

        protected:
            bool cast_shadows_;
            bool set_up_;
            int  pcf_;
            unsigned int framebuffer_;
            std::vector<graphics::Texture> shadow_maps_;
        };
    }
}

#endif