#include <directional_light.hpp>

namespace nitro
{
    namespace core
    {
        DirectionalLight::DirectionalLight()
          // Directions (vectors) actually have w = 0. However, if we want to translate
          // the light using a matrix tranform we need to set the w component
          // to w = 1 otherwise, the translation won't have any effect; 
        : Shadow{2, 5},
          direction_{0.0f,0.0f,0.0f,1.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          transform_{},
          projection_{}
        {

        }

        DirectionalLight::DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color)
        : Shadow{2, 5},
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

            shadow_maps_[0] = graphics::Texture{
                "shadow_map", 
                constants::SHADOW_WIDTH, 
                constants::SHADOW_HEIGHT, 
                GL_TEXTURE_2D, 
                GL_RG32F, 
                GL_RGBA, 
                GL_FLOAT};
            
            shadow_maps_[1] = graphics::Texture{
                "shadow_map", 
                constants::SHADOW_WIDTH, 
                constants::SHADOW_HEIGHT, 
                GL_TEXTURE_2D, 
                GL_DEPTH24_STENCIL8, 
                GL_DEPTH_STENCIL, 
                GL_UNSIGNED_INT_24_8};
            
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_maps_[0].TextureReference(), 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, shadow_maps_[1].TextureReference(), 0);
            
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Directional light framebuffer is not complete \n");
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, 0);

            set_up_ = true;
        }

        void DirectionalLight::DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                           const std::vector<std::shared_ptr<Actor>>& actors)
        {   
            auto shader = shaders.at("directional_shadows");
            shader.Use();

            if(!set_up_)
                SetupShadows();
            
            glViewport(0, 0,  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            projection_ = clutch::Orthopraphic(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
            transform_  = clutch::LookAt(direction_ * -1.0f, {0.0f, 0.0f,  0.0f, 0.0f}, {0.0f, 1.0f,  0.0f, 0.0f}); 

            shader.SetUniformMat4("uLightTransform", projection_ * transform_);

            for(const auto& actor : actors)
                actor->Draw(shader, false);
        }

        void DirectionalLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("uLightDir",     direction_);
            shader.SetUniform4f("uLightColor",   color_);
            shader.SetUniformInt("uCastsShadow", cast_shadows_);
            shader.SetUniformInt("uPCF",         pcf_);
            shader.SetUniformMat4("uLightTransform",  projection_ * transform_);
        
            shadow_maps_[0].Draw(shader);
        } 

        void DirectionalLight::Erase() 
        {

        }

        void DirectionalLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
