#include <light.hpp>

namespace nitro
{
  namespace core
  {
    PointLight::PointLight(const std::string& model,
                           const clutch::Vec3<float>& position, 
                           const clutch::Vec3<float>& color,
                           const float max_distance)
    : Actor{model, {"light"}},
      position_{position},
      color_{color},
      max_distance_{max_distance}
    {
    }

    PointLight::PointLight()
    : Actor{"planet/planet.obj", {"light"}},
      position_{0.0f,0.0f,0.0},
      color_{1.0f,1.0f,1.0f},
      max_distance_{20.f}
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
  }
}