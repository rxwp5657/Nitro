#include <camera.hpp>

namespace nitro
{
    namespace core
    {
        Camera::Camera(const clutch::Vec4<float>& position, 
                       const clutch::Vec4<float>& target,
                       const clutch::Vec4<float>& up,
                       const clutch::Mat4<float>& projection)
        : position_{position},
          target_{target},
          up_{up},
          projection_{projection}
        {    
        }
        
        clutch::Mat4<float> Camera::ViewTransform()
        {
          return clutch::LookAt(position_, target_, up_);
        }

        void Camera::Erase()
        {

        }

        void Camera::Setup(const graphics::Shader& shader)
        {

        }

        void Camera::Draw(const  graphics::Shader& shader)
        {
            shader.SetUniformMat4("uView", ViewTransform());
            shader.SetUniformMat4("uProj", projection_);
        }
    }
}