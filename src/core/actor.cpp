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

        void Actor::Rotate(const clutch::Mat4<float> r)
        {
            rotation_ = r;
        }

        void Actor::Scale(const clutch::Mat4<float> s)
        {
            scaling_ = s;
        }

        void Actor::Translate(const clutch::Mat4<float> t)
        {
            translation_ = t;
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
            return clutch::Transpose(clutch::Inverse(translation_ * scaling_ * rotation_));
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