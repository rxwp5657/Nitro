#ifndef POINT_LIGHT
#define POINT_LIGHT

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vec4.hpp>
#include <lookat.hpp>
#include <transforms.hpp>
#include <projections.hpp>

#include "../graphics/texture.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/drawable.hpp"
#include "./actor.hpp"
#include "./constants.hpp"
#include "./shadows.hpp"


namespace nitro 
{
    namespace core
    {
        class PointLight : public graphics::Drawable, public Shadow
        {
        public:

            PointLight();
            PointLight(const clutch::Vec4<float>& position, 
                       const clutch::Vec4<float>& color,
                       const float max_distance);

            void Range(float range);
            void Move(float x, float y, float z);
            void CastShadows(bool casts);

            void Draw(const graphics::Shader& shader, bool default_framebuffer = true) override;
            void Erase() override;

            void DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                             const std::vector<std::shared_ptr<Actor>>& actors) override; 
            void SetupShadows() override;

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> color_;
            float max_distance_;
        
            void Setup(const graphics::Shader& shader) override;
            clutch::Mat4<float> FaceTransform(const clutch::Vec4<float>& direction, const clutch::Vec4<float>& up);
        };
    }
}


#endif