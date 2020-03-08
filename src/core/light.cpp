#include <light.hpp>

namespace nitro
{
    namespace core
    {
        PointLight::PointLight(const clutch::Vec4<float>& position,
                               const clutch::Vec4<float>& color,
                               const float max_distance)
        : position_{position},
          color_{color},
          max_distance_{max_distance}
        {
        
        }

        PointLight::PointLight()
        : position_{0.0f, 0.0f, 0.0f, 1.0f},
          color_{1.0f, 1.0f, 1.0f, 1.0f},
          max_distance_{20.f}
        {

        }

        void PointLight::Range(float range)
        {
          max_distance_ = range;
        }

        void PointLight::Move(const clutch::Mat4<float>& transform)
        {
          position_ = transform * position_;
        }
    }
}