#include "../../include/core/spot_light.hpp"

namespace nitro
{
    namespace core
    {
        SpotLight::SpotLight()
        : Shadow{2,5},
          position_{0.0f, 0.0f, 0.0f, 1.0f},
          direction_{0.0f,0.0f,-1.0f, 1.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          transform_{},
          projection_{},
          cutoff_{cos((clutch::PI   * 65.0f) / 180.0f)},
          max_distance_{20.0f},
          umbra_{cos((clutch::PI    * 40.5f) / 180.0f)},
          penumbra_{cos((clutch::PI * 20.5f) / 180.0f)}
        { 

        }

        SpotLight::SpotLight(clutch::Vec4<float> position,
                             clutch::Vec4<float> direction,
                             clutch::Vec4<float> color,
                             float cutoff,
                             float umbra,
                             float penumbra,
                             float max_distance)
        : Shadow{2,5}, 
          position_{position},
          direction_{direction},
          color_{color},
          transform_{},
          projection_{},
          cutoff_{cutoff},
          max_distance_{max_distance},
          umbra_{umbra},
          penumbra_{penumbra}
        {

        }

        void SpotLight::Umbra(float umbra)
        {
          umbra_ = umbra;
        }

        void SpotLight::Penumbra(float penumbra)
        {
          penumbra_ = penumbra;
        }

        void SpotLight::Distance(float distance)
        {
            max_distance_ = distance;
        }

        void SpotLight::Cutoff(float cutoff)
        {
            cutoff_ = cutoff;
        }

        void SpotLight::MovePos(float x, float y, float z)
        {
            position_ = clutch::Translation(x,y,z) * position_;
        }

        void SpotLight::MoveDir(float x, float y, float z)
        {
            direction_ = clutch::Translation(x,y,z) * direction_;
        }

        void SpotLight::SetupShadows()
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
                throw std::runtime_error("Spot light framebuffer is not complete \n");
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, 0);

            set_up_ = true;
        }

        void SpotLight::DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                    const std::vector<std::shared_ptr<Actor>>& actors)
        {
            auto shader = shaders.at("spot_shadows");

            shader.Use();

            if(!set_up_)
                SetupShadows();

            glViewport(0, 0,  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            projection_ = clutch::Perspective((90.0f * clutch::PI) / 180.0f, 1.5f, 1.0f, max_distance_);
            transform_  = clutch::LookAt(position_, position_ + direction_, clutch::Vec4<float>{0.0f, 1.0f, 0.0f, 0.0f}); 

            shader.SetUniformMat4("uLightTransform", projection_ * transform_);

            for(const auto& actor : actors)
                actor->Draw(shader, false);
        }

        void SpotLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("uLightPos",          position_);
            shader.SetUniform4f("uLightDir",          direction_);
            shader.SetUniform4f("uLightColor",        color_);
            shader.SetUniformMat4("uLightTransform",  projection_ * transform_);
            shader.SetUniformFloat("uCutoff",         cutoff_);
            shader.SetUniformFloat("uMaxDistance",    max_distance_);
            shader.SetUniformFloat("uUmbra",          umbra_);
            shader.SetUniformFloat("uPenumbra",       penumbra_);
            shader.SetUniformInt("uCastsShadow",      cast_shadows_);
            shader.SetUniformInt("uPCF",              pcf_);

            shadow_maps_[0].Draw(shader);
        }

        void SpotLight::Erase()
        {

        }

        void SpotLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
