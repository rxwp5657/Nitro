#include <actor.hpp>

namespace nitro
{
    namespace core
    {
        Actor::Actor(const std::string& model,
                     std::initializer_list<std::string> shaders)
        : model_{model},
          translation_{clutch::Mat4<float>{}},
          rotation_{clutch::Mat4<float>{}},
          scaling_{clutch::Mat4<float>{}},
          shaders_{shaders}
        {  
        }

        Actor::Actor(const graphics::Model& model,
                     std::initializer_list<std::string> shaders)
        : model_{model},
          translation_{clutch::Mat4<float>{}},
          rotation_{clutch::Mat4<float>{}},
          scaling_{clutch::Mat4<float>{}},
          shaders_{shaders}
        {  
        }

        void Actor::Rotate(float x, float y, float z)
        {
            rotation_ = clutch::RotateX(x) * clutch::RotateY(y) * clutch::RotateZ(z);
        }

        void Actor::Scale(float x, float y, float z)
        {
            scaling_ = clutch::Scale(x,y,z);
        }

        void Actor::Translate(float x, float y, float z)
        {
            translation_ = clutch::Translation(x,y,z);
        }

        void Actor::FlipUV()
        {
            model_.FlipUV();
        }

        clutch::Mat4<float> Actor::Model() const
        {
            return translation_ * (scaling_ * rotation_);
        }

        clutch::Mat4<float> Actor::NormalMat() const
        {   
            return clutch::Transpose(clutch::Inverse(translation_ * (scaling_ * rotation_)));
        }

        void Actor::Erase()
        {
            
        }

        void Actor::Setup(const graphics::Shader& shader)
        {
            shader.SetUniformMat4("uModel",Model());
            shader.SetUniformMat4("uNormalMat", NormalMat());
        }

        void Actor::Draw(const graphics::Shader& shader)
        {
            Setup(shader);
            model_.Draw(shader);
        }

        void Actor::AddShader(const std::string& shader_name)
        {
            shaders_.push_back(shader_name);
        }

        std::vector<std::string> Actor::Shaders() const
        {
            return shaders_;
        }

    }
}