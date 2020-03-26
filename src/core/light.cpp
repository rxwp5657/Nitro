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
          max_distance_{5.0f}
        {

        }

        void PointLight::Range(float range)
        {
          max_distance_ = range;
        }

        void PointLight::Move(float x, float y, float z)
        {
          position_ = clutch::Translation(x,y,z) * position_;
        }

        SpotLight::SpotLight()
        : position_{0.0f, 0.0f, 0.0f, 1.0f},
          direction_{0.0f,0.0f,-1.0f,0.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          cutoff_{cos((clutch::PI * 90.0f) / 180.0f)},
          max_distance_{5.0f},
          umbra_{cos((clutch::PI * 40.5f) / 180.0f)},
          penumbra_{cos((clutch::PI * 10.5f) / 180.0f)}
        {

        }

        SpotLight::SpotLight(clutch::Vec4<float> position,
                             clutch::Vec4<float> direction,
                             clutch::Vec4<float> color,
                             float cutoff,
                             float max_distance,
                             float umbra,
                             float penumbra)
        : position_{position},
          direction_{direction},
          color_{color},
          cutoff_{cutoff},
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

        void SpotLight::MovePos(float x, float y, float z)
        {
            position_ = clutch::Translation(x,y,z) * position_;
        }

        void SpotLight::MoveDir(float x, float y, float z)
        {
          direction_ = clutch::Translation(x,y,z) * direction_;
        }

        DirectionalLight::DirectionalLight()
          // Directions (vectors) actually have w = 0. However, if we want to translate
          // the light using a matrix tranform we need to set the w component
          // to w = 1 otherwise, the translation won't have any effect; 
        : direction_{0.0f,0.0f,0.0f,1.0f},
          color_{1.0f,1.0f,1.0f,1.0f}
        {

        }

        void DirectionalLight::Move(float x, float y, float z)
        {
            direction_ = clutch::Translation(x,y,z) * direction_;
        }
    }
}