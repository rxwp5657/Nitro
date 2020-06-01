#include <point_light.hpp>

namespace nitro
{
    namespace core
    {
        PointLight::PointLight(const clutch::Vec4<float>& position,
                               const clutch::Vec4<float>& color,
                               const float max_distance)
        : Shadow{1, 5},
          position_{position},
          color_{color},
          max_distance_{max_distance}
        {
        
        }

        PointLight::PointLight()
        : Shadow{1, 5},
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

        void PointLight::SetupShadows()
        {
            glGenFramebuffers(1, &framebuffer_);
            
            shadow_maps_[0] = graphics::Texture{
                    "shadow_map",
                    constants::SHADOW_WIDTH, 
                    constants::SHADOW_HEIGHT, 
                    GL_TEXTURE_CUBE_MAP, 
                    GL_DEPTH_COMPONENT, 
                    GL_DEPTH_COMPONENT, 
                    GL_FLOAT};

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_maps_[0].TextureReference(), 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, 0);

            set_up_= true;
        }

        void PointLight::DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                     const std::vector<std::shared_ptr<Actor>>& actors)
        {    
            auto shader = shaders.at("point_shadows");
            shader.Use();
        
            if(!set_up_)
                SetupShadows();
            
            glViewport(0, 0,  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            auto projection = clutch::Perspective((90.0f * clutch::PI) / 180.0f, (float)constants::SHADOW_WIDTH / (float)constants::SHADOW_HEIGHT, 0.5f, max_distance_);

            shader.SetUniform4f("uLightPos", position_);
            std::vector<clutch::Mat4<float>> transforms{};
            transforms.push_back(projection * FaceTransform({ 1.0f, 0.0f, 0.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({-1.0f, 0.0f, 0.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f,-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f,-1.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f, 0.0f, 1.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f, 0.0f,-1.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));

            for(int i = 0; i < 6; i++)
                shader.SetUniformMat4("face_vp[" + std::to_string(i) + "]", transforms[i]);    
            
            shader.SetUniformFloat("uFarPlane", max_distance_);
            
            for(const auto& actor : actors)
                actor->Draw(shader, false);
            
        }

        void PointLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("uLightPos",       position_);
            shader.SetUniform4f("uLightColor",     color_);
            shader.SetUniformFloat("uMaxDistance", max_distance_);
            shader.SetUniformInt("uCastsShadow",   cast_shadows_);
            shader.SetUniformInt("uPCF",           pcf_);
            
            if(cast_shadows_)
                shadow_maps_[0].Draw(shader);
            else
            {
                glActiveTexture(GL_TEXTURE12);
                shader.SetUniformInt("shadow_map", 12);
                glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_maps_[0].TextureReference());
            }
        }

        void PointLight::Erase()
        {

        }

        void PointLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
