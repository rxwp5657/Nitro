#ifndef CAMERA_H
#define CAMERA_H

#include <assert.h>
#include <vec4.hpp>
#include <mat4.hpp>
#include <lookat.hpp>
#include <drawable.hpp>
#include <shader.hpp>
#include <projections.hpp>
#include <keyboard_evnt.hpp>

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
                   const clutch::Mat4<float>& projection,
                   float speed = 3.5);
            
            Camera();
        
            clutch::Mat4<float> ViewTransform();

            void Forward(const input::KeyBoardEvent& event);
            void Backward(const input::KeyBoardEvent& event);
            void Left(const input::KeyBoardEvent& event);
            void Right(const input::KeyBoardEvent& event);

            void Look(const clutch::Vec4<float> front);

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const  graphics::Shader& shader, bool default_framebuffer = true) override;

        private:
            clutch::Vec4<float> position_; 
            clutch::Vec4<float> target_;
            clutch::Vec4<float> up_;
            clutch::Mat4<float> projection_;
            float speed_;
        };
    }
}

#endif