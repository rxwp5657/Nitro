#include <actor.hpp>

namespace nitro
{
    namespace core
    {
        Actor::Actor(const std::string& model)
        : model_{model},
          transform_{clutch::Mat4<float>{}}
        {  
        }

        clutch::Mat4<float> Actor::Transform() const
        {
            return transform_;
        }
        
        void Actor::Transform(const clutch::Mat4<float> t)
        {
            transform_ = t;
        }

        void Actor::Erase()
        {
            
        }

        void Actor::Setup(const graphics::Shader& shader)
        {
            shader.SetUniformMat4("uModel",transform_);
        }

        void Actor::Draw(const graphics::Shader& shader)
        {
            Setup(shader);
            model_.Draw(shader);
        }

    }
}