#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vec4.hpp>
#include <mat4.hpp>
#include <transforms.hpp>
#include <lookat.hpp>
#include <projections.hpp>

#include "./actor.hpp"
#include "./shadows.hpp"
#include "./constants.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/drawable.hpp"
#include "../graphics/texture.hpp"

namespace nitro 
{
    namespace core
    {
        class DirectionalLight : public graphics::Drawable, public Shadow
        {
        public:
            DirectionalLight();
            DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color);

            void Move(float x, float y, float z);

            void Draw(const graphics::Shader& shader, bool default_framebuffer = true) override;
            void Erase() override;

            void DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                             const std::vector<std::shared_ptr<Actor>>& actors) override;
             
            void SetupShadows() override;

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