#include <light.hpp>

namespace nitro
{
    namespace core
    {
      PointLight::PointLight(const std::string& model,
                             const clutch::Vec3<float>& position, 
                             const clutch::Vec3<float>& color,
                             const float max_distance)
      : model_{model},
        position_{position},
        color_{color},
        max_distance_{max_distance}
      {

      }

        void PointLight::Erase()
        {

        }

        void PointLight::Setup(const graphics::Shader& shader)
        {
            shader.SetUniform3f("lightPos",  position_);
            shader.SetUniform3f("lightColor",color_);
            shader.SetUniformFloat("lightDistance", max_distance_);
        }

        void PointLight::Draw(const graphics::Shader& shader)
        {
            Setup(shader);
            model_.Draw(shader);
        }
    }
}