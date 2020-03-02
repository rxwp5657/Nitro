#include <camera.hpp>
#include <projections.hpp>

namespace nitro
{
    namespace core
    {
        Camera::Camera(const clutch::Vec4<float>& position, 
                       const clutch::Vec4<float>& target,
                       const clutch::Vec4<float>& up,
                       const clutch::Mat4<float>& projection,
                       float speed)
        : position_{position},
          target_{target},
          up_{up},
          projection_{projection},
          speed_{speed}
        {    
        }

        Camera::Camera()
        : position_{0.0f, 0.0f, 5.0f, 1.0},
          target_{0.0f, 0.0f, -1.0f, 1.0},
          up_{0.0f, 1.0f, 0.0f, 0.0},
          projection_{clutch::Perspective((45.0f * clutch::PI) / 180, 800.0f / 600.0f, 1.0f, 100.0f)},
          speed_{3.5}
        {

        }
        
        clutch::Mat4<float> Camera::ViewTransform()
        {
          return clutch::LookAt(position_, target_, up_);
        }

        void Camera::Forward(const input::KeyBoardEvent& event)
        {
            const auto front = target_ - position_;
            position_ += (front * speed_ * event.time);
        }

        void Camera::Backward(const input::KeyBoardEvent& event)
        {
            const auto front = target_ - position_;
            position_ -= (front * speed_ * event.time);
        }

        void Camera::Left(const input::KeyBoardEvent& event)
        {
            const auto front = target_ - position_;
            const auto left  = clutch::Normalize(clutch::Cross(front, up_));
            target_   -= (left * speed_ * event.time);
            position_ -= (left * speed_ * event.time);
        }

        void Camera::Right(const input::KeyBoardEvent& event)
        {
            const auto front = target_ - position_;
            const auto left  = clutch::Normalize(clutch::Cross(front, up_));
            target_   += (left * speed_ * event.time);
            position_ += (left * speed_ * event.time);
        }

        void Camera::Look(const clutch::Vec4<float> front)
        {
            target_ = position_ + front;
        }

        void Camera::Erase(){}
        void Camera::Setup(const graphics::Shader& shader){}

        void Camera::Draw(const  graphics::Shader& shader)
        {
            shader.SetUniformMat4("uView", ViewTransform());
            shader.SetUniformMat4("uProj", projection_);
            shader.SetUniform4f("viewPos", position_);
        }
    }
}