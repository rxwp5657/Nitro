#include <light.hpp>

namespace nitro
{
    namespace core
    {
      PointLight::PointLight(const std::string& model,
                             const clutch::Vec3<float>& position, 
                             const clutch::Vec3<float>& color,
                             const float max_distance,
                             clutch::Mat4<float> transform)
      : model_{model},
        position_{position},
        color_{color},
        max_distance_{max_distance},
        transform_{transform}
      {

      }

      PointLight::PointLight()
      : model_{"planet/planet.obj"},
        position_{0.0f,1.5f,0.0},
        color_{3.0f,3.0f,3.0f},
        max_distance_{20.f},
        transform_{clutch::Mat4<float>{}}
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
          shader.SetUniformMat4("uModel",transform_);
      }

      void PointLight::Draw(const graphics::Shader& shader)
      {
          Setup(shader);
          model_.Draw(shader);
      }

      clutch::Mat4<float> PointLight::Transform() const
      {
          return transform_;
      }
      
      void PointLight::Transform(const clutch::Mat4<float> t)
      {
          transform_ = t;
      }
  }
}