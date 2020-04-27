#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec4.hpp>
#include <mat4.hpp>
#include <transforms.hpp>
#include <texture.hpp>
#include <constants.hpp>
#include <drawable.hpp>
#include <shader.hpp>
#include <light_shadow.hpp>

namespace nitro 
{
    namespace core
    {
        class DirectionalLight : public graphics::Drawable, public LightShadow
        {
        public:
            DirectionalLight();
            DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color);

            void Move(float x, float y, float z);

            void Draw(const graphics::Shader& shader) override;
            void Erase() override;

            void DrawShadows(const graphics::Shader& shader, const graphics::Framebuffer& buffer) override; 

        private:
            clutch::Vec4<float> direction_;
            clutch::Vec4<float> color_;
            clutch::Mat4<float> transform_;
            clutch::Mat4<float> projection_;

            void Setup(const graphics::Shader& shader) override;
        };
    }
}


#endif