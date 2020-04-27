#include <point_light.hpp>

namespace nitro
{
    namespace core
    {
        PointLight::PointLight(const clutch::Vec4<float>& position,
                               const clutch::Vec4<float>& color,
                               const float max_distance)
        : LightShadow{false, 
                      graphics::Texture{
                      constants::SHADOW_WIDHT, 
                      constants::SHADOW_HEIGHT, 
                      "shadow_map",
                      GL_TEXTURE_CUBE_MAP,
                      GL_DEPTH_COMPONENT,
                      true}},
          position_{position},
          color_{color},
          max_distance_{max_distance}
        {
        
        }

        PointLight::PointLight()
        : LightShadow{false, 
                      graphics::Texture{
                      constants::SHADOW_WIDHT, 
                      constants::SHADOW_HEIGHT, 
                      "shadow_map",
                      GL_TEXTURE_CUBE_MAP,
                      GL_DEPTH_COMPONENT,
                      true}},
          position_{0.0f, 0.0f, 0.0f, 1.0f},
          color_{1.0f, 1.0f, 1.0f, 1.0f},
          max_distance_{15.0f}
        {

        }

        void PointLight::Range(float range)
        {
            max_distance_ = range;
        }

        void PointLight::Move(float x, float y, float z)
        {
            position_ = clutch::Translation(x,y,z) * position_;
        }

        clutch::Mat4<float> PointLight::FaceTransform(const clutch::Vec4<float>& direction, const clutch::Vec4<float>& up)
        {
            return clutch::LookAt(position_, position_ + direction, up);
        }

        void PointLight::DrawShadows(const graphics::Shader& shader, const graphics::Framebuffer& buffer)
        {
            
            buffer.AttachTexture(GL_DEPTH_ATTACHMENT, shadow_map_.TextureReference());

            glViewport(0, 0, constants::SHADOW_WIDHT, constants::SHADOW_HEIGHT);
            buffer.Bind();
            
            glClear(GL_DEPTH_BUFFER_BIT);
            shader.Use();
            
            auto projection = clutch::Perspective((90.0f * clutch::PI) / 180.0f, 1.0f, 1.0, max_distance_);

            shader.SetUniform4f("light_pos", position_);
            shader.SetUniformMat4("face_vp[ " + std::to_string(0) + "]", projection * FaceTransform({0.0f, 0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}));
            shader.SetUniformMat4("face_vp[ " + std::to_string(1) + "]", projection * FaceTransform({0.0f, 0.0f,  1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}));
            shader.SetUniformMat4("face_vp[ " + std::to_string(2) + "]", projection * FaceTransform({1.0f, 0.0f,  0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}));
            shader.SetUniformMat4("face_vp[ " + std::to_string(3) + "]", projection * FaceTransform({-1.0f,0.0f,  0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}));
            shader.SetUniformMat4("face_vp[ " + std::to_string(4) + "]", projection * FaceTransform({0.0f, 1.0f,  0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}));
            shader.SetUniformMat4("face_vp[ " + std::to_string(5) + "]", projection * FaceTransform({0.0f,-1.0f,  0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}));    
            
        }

        void PointLight::Draw(const graphics::Shader& shader)
        {
            shader.SetUniform4f("light_pos",   position_);
            shader.SetUniform4f("light_color", color_);
            shader.SetUniformFloat("max_distance", max_distance_);
            shader.SetUniformInt("cast_shadow", shadows_);
            shadow_map_.TextureUnit(GL_TEXTURE11, 11, 0);
        }

        void PointLight::Erase()
        {

        }

        void PointLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
