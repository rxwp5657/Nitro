#ifndef POINT_LIGHT
#define POINT_LIGHT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec4.hpp>
#include <transforms.hpp>
#include <texture.hpp>
#include <shader.hpp>
#include <drawable.hpp>
#include <constants.hpp>
#include <light_shadow.hpp>
#include <projections.hpp>
#include <lookat.hpp>


namespace nitro 
{
    namespace core
    {
        class PointLight : public graphics::Drawable, public LightShadow
        {
        public:

            PointLight();
            PointLight(const clutch::Vec4<float>& position, 
                       const clutch::Vec4<float>& color,
                       const float max_distance);

            void Range(float range);
            void Move(float x, float y, float z);
            void CastShadows(bool casts);

            void Draw(const graphics::Shader& shader) override;
            void Erase() override;

            void DrawShadows(const graphics::Shader& shader, const graphics::Framebuffer& buffer) override; 

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