#include <directional_light.hpp>

namespace nitro
{
    namespace core
    {
        DirectionalLight::DirectionalLight()
          // Directions (vectors) actually have w = 0. However, if we want to translate
          // the light using a matrix tranform we need to set the w component
          // to w = 1 otherwise, the translation won't have any effect; 
        : LightShadow{false, false},
          direction_{0.0f,0.0f,0.0f,1.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          transform_{},
          projection_{}
        {

        }

        DirectionalLight::DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color)
        : LightShadow{false, false},
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

        void DirectionalLight::SetupShadows()
        {
            glGenFramebuffers(1, &framebuffer_);
            glGenTextures(1,     &shadow_map_);

            glBindTexture(GL_TEXTURE_2D, shadow_map_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Directional light framebuffer is not complete \n");
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ShadowSetup(true);
        }

        void DirectionalLight::DrawShadows(const graphics::Shader& shader)
        {   
            if(!ShadowSetup())
                SetupShadows();
            
            glViewport(0, 0,  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            projection_ = clutch::Orthopraphic(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
            transform_  = clutch::LookAt(direction_ * -1.0f, {0.0f, 0.0f,  0.0f, 0.0f}, {0.0f, 1.0f,  0.0f, 0.0f}); 

            shader.SetUniformMat4("light_transform", projection_ * transform_);

        }

        void DirectionalLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("light_dir",   direction_);
            shader.SetUniform4f("light_color", color_);
            shader.SetUniformMat4("light_transform",  projection_ * transform_);
            shader.SetUniformInt("cast_shadow", shadows_);
        
            glActiveTexture(GL_TEXTURE12);
            shader.SetUniformInt("shadow_map", 12);
            glBindTexture(GL_TEXTURE_2D, shadow_map_);
        } 

        void DirectionalLight::Erase() 
        {

        }

        void DirectionalLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
