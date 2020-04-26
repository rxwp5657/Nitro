#include <spot_light.hpp>

namespace nitro
{
    namespace core
    {
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

        void SpotLight::Distance(float distance)
        {
            max_distance_ = distance;
        }

        void SpotLight::Cutoff(float cutoff)
        {
            cutoff_ = cutoff;
        }

        void SpotLight::MovePos(float x, float y, float z)
        {
            position_ = clutch::Translation(x,y,z) * position_;
        }

        void SpotLight::MoveDir(float x, float y, float z)
        {
          direction_ = clutch::Translation(x,y,z) * direction_;
        }
    }
}
