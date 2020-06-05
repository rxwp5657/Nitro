#include "../../include/core/shadows.hpp"

namespace nitro
{
    namespace core
    {
        Shadow::Shadow(int num_shadow_maps, int pcf)
        :   cast_shadows_{false}, 
            set_up_{false}, 
            pcf_{pcf}, 
            framebuffers_(2), 
            shadow_maps_(num_shadow_maps) 
        {

        }

        bool Shadow::CastShadow() const
        { 
            return cast_shadows_; 
        }

        void Shadow::CastShadow(bool cast)
        { 
            cast_shadows_ = cast; 
        }

        graphics::Texture Shadow::ShadowTexture(int index) const
        {
            return shadow_maps_[index];
        }

        unsigned int Shadow::ShadowMap(int index) const
        {
            return shadow_maps_[index].TextureReference();
        }
    }
}