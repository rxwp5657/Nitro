#include <directional_light.hpp>

namespace nitro
{
    namespace core
    {
        DirectionalLight::DirectionalLight()
          // Directions (vectors) actually have w = 0. However, if we want to translate
          // the light using a matrix tranform we need to set the w component
          // to w = 1 otherwise, the translation won't have any effect; 
        : LightShadow{false, 
                      graphics::Texture{
                      constants::SHADOW_WIDHT,
                      constants::SHADOW_HEIGHT, 
                      "shadow_map",
                      GL_TEXTURE_2D,
                      GL_DEPTH_COMPONENT}},
          direction_{0.0f,0.0f,0.0f,1.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          transform_{},
          projection_{}
        {

        }

        DirectionalLight::DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color)
        : LightShadow{false, 
                      graphics::Texture{
                      constants::SHADOW_WIDHT,
                      constants::SHADOW_HEIGHT, 
                      "shadow_map",
                      GL_TEXTURE_2D,
                      GL_DEPTH_COMPONENT}},
          direction_{direction},
          color_{color},
          transform_{},
          projection_{}
        {

        }

        void DirectionalLight::Move(float x, float y, float z)
        {
            direction_ = clutch::Translation(x,y,z) * direction_;
        }

        void DirectionalLight::DrawShadows(const graphics::Shader& shader, const graphics::Framebuffer& buffer)
        {   
            /*
            buffer.AttachTexture(GL_DEPTH_ATTACHMENT, shadow_map_.TextureReference());
            glViewport(0, 0, constants::SHADOW_WIDHT, constants::SHADOW_HEIGHT);
            buffer.Bind();
            glClear(GL_DEPTH_BUFFER_BIT);
            shader.Use();
            */
        }

        void DirectionalLight::Draw(const graphics::Shader& shader)
        {
            shader.SetUniform4f("light_dir",   direction_);
            shader.SetUniform4f("light_color", color_);
            shader.SetUniformMat4("light_transform",  transform_);
            shader.SetUniformMat4("light_projection", projection_);
        } 

        void DirectionalLight::Erase() 
        {

        }

        void DirectionalLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
