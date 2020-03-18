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

        SpotLight::SpotLight()
        : position_{0.0f,0.0f,0.0f,1.0f},
          direction_{0.0f,0.0f,1.0f,0.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          max_distance_{0.5f},
          umbra_{cos((clutch::PI * 17.5f) / 180.0f)},
          penumbra_{cos((clutch::PI * 12.5f) / 180.0f)}
        {

        }

        SpotLight::SpotLight(clutch::Vec4<float> position,
                             clutch::Vec4<float> direction,
                             clutch::Vec4<float> color,
                             float max_distance,
                             float umbra,
                             float penumbra)
        : position_{position},
          direction_{direction},
          max_distance_{max_distance},
          umbra_{umbra},
          penumbra_{penumbra}
        {

        }

        void SpotLight::Umbra(float umbra)
        {
          umbra_ = umbra;
        }

        void SpotLight::Penumbra(float penumbra)
        {
          penumbra_ = penumbra;
        }

        void SpotLight::MovePos(const clutch::Mat4<float>& transform)
        {
            position_ = transform * position_;
        }

        void SpotLight::MoveDir(const clutch::Mat4<float>& transform)
        {
          direction_ = transform * direction_;
        }

        DirectionalLight::DirectionalLight()
        : direction_{0.0f,0.0f,1.0f,0.0f},
          color_{1.0f,1.0f,1.0f,1.0f}
        {

        }

        void DirectionalLight::Move(const clutch::Mat4<float>& transform)
        {
            direction_ = transform * direction_;
        }
    }
}