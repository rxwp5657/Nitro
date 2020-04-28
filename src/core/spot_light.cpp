#include <spot_light.hpp>

namespace nitro
{
    namespace core
    {
        SpotLight::SpotLight()
        : LightShadow{false, false},
          position_{0.0f, 0.0f, 0.0f, 1.0f},
          direction_{0.0f,0.0f,-1.0f,0.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          transform_{},
          projection_{},
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
        : LightShadow{false,false}, 
          position_{position},
          direction_{direction},
          color_{color},
          transform_{},
          projection_{},
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

        void SpotLight::SetupShadows()
        {
          
        }

        void SpotLight::DrawShadows(const graphics::Shader& shader)
        {
            

        }

        void SpotLight::Draw(const graphics::Shader& shader)
        {
            shader.SetUniform4f("light_pos",          position_);
            shader.SetUniform4f("light_dir",          direction_);
            shader.SetUniform4f("light_color",        color_);
            shader.SetUniformMat4("light_transform",  transform_);
            shader.SetUniformMat4("light_projection", projection_);
            shader.SetUniformFloat("cutoff",          cutoff_);
            shader.SetUniformFloat("max_distance",    max_distance_);
            shader.SetUniformFloat("umbra",           umbra_);
            shader.SetUniformFloat("penumbra",        penumbra_);
        }

        void SpotLight::Erase()
        {

        }

        void SpotLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
