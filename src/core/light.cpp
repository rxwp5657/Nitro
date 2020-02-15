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
      max_distance_{max_distance},
      translation_{clutch::Mat4<float>{}},
      rotation_{clutch::Mat4<float>{}},
      scaling_{clutch::Mat4<float>{}}
    {

    }

    PointLight::PointLight()
    : model_{"planet/planet.obj"},
      position_{0.0f,1.5f,0.0},
      color_{3.0f,3.0f,3.0f},
      max_distance_{20.f},
      translation_{clutch::Mat4<float>{}},
      rotation_{clutch::Mat4<float>{}},
      scaling_{clutch::Mat4<float>{}}
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
        shader.SetUniformMat4("uModel",Model());
    }

    void PointLight::Draw(const graphics::Shader& shader)
    {
        Setup(shader);
        model_.Draw(shader);
    }

    void PointLight::Translate(const clutch::Mat4<float> t)
    {
      translation_ = t;
    }

    void PointLight::Rotate(const clutch::Mat4<float> r)
    {
        rotation_ = r;
    }

    void PointLight::Scale(const clutch::Mat4<float> s)
    {
        scaling_ = s;
    }

    clutch::Mat4<float> PointLight::Model() const
    {
        return translation_ * (scaling_ * rotation_);
    }
  }
}