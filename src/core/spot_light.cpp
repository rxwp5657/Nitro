#include <spot_light.hpp>

namespace nitro
{
    namespace core
    {
        SpotLight::SpotLight()
        : LightShadow{false, false},
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
                             float max_distance,
                             float umbra,
                             float penumbra)
        : LightShadow{false,false}, 
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
            glGenTextures(1, &shadow_map_);

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
                throw std::runtime_error("Spot light framebuffer is not complete \n");
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ShadowSetup(true);
        }

        void SpotLight::DrawShadows(const graphics::Shader& shader)
        {
            if(!ShadowSetup())
                SetupShadows();
          
            glViewport(0, 0,  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            projection_ = clutch::Perspective((90.0f * clutch::PI) / 180.0f, 1.5f, 1.0f, max_distance_);
            transform_  = clutch::LookAt(position_, position_ + direction_, clutch::Vec4<float>{0.0f, 1.0f, 0.0f, 0.0f}); 

            shader.SetUniformMat4("light_transform", projection_ * transform_);
        }

        void SpotLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("light_pos",          position_);
            shader.SetUniform4f("light_dir",          direction_);
            shader.SetUniform4f("light_color",        color_);
            shader.SetUniformMat4("light_transform",  projection_ * transform_);
            shader.SetUniformFloat("cutoff",          cutoff_);
            shader.SetUniformFloat("max_distance",    max_distance_);
            shader.SetUniformFloat("umbra",           umbra_);
            shader.SetUniformFloat("penumbra",        penumbra_);
            shader.SetUniformInt("cast_shadow",       shadows_);

            glActiveTexture(GL_TEXTURE12);
            shader.SetUniformInt("shadow_map", 12);
            glBindTexture(GL_TEXTURE_2D, shadow_map_);
        }

        void SpotLight::Erase()
        {

        }

        void SpotLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
