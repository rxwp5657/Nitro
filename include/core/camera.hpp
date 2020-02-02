#ifndef CAMERA_H
#define CAMERA_H

#include <vec3.hpp>
#include <mat4.hpp>
#include <lookat.hpp>
#include <drawable.hpp>
#include <shader.hpp>

namespace nitro
{
    namespace core
    {
        class Camera : public graphics::Drawable
        {
        public:
            Camera(const clutch::Vec4<float>& position, 
                   const clutch::Vec4<float>& target,
                   const clutch::Vec4<float>& up,
                   const clutch::Mat4<float>& projection);
        
            clutch::Mat4<float> ViewTransform();

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const  graphics::Shader& shader) override;

        private:
            clutch::Vec4<float> position_; 
            clutch::Vec4<float> target_;
            clutch::Vec4<float> up_;
            clutch::Mat4<float> projection_;
        };
    }
}

#endif