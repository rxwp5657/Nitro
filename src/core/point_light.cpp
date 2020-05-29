#include <point_light.hpp>

namespace nitro
{
    namespace core
    {
        PointLight::PointLight(const clutch::Vec4<float>& position,
                               const clutch::Vec4<float>& color,
                               const float max_distance)
        : LightShadow{false, false},
          position_{position},
          color_{color},
          max_distance_{max_distance}
        {
        
        }

        PointLight::PointLight()
        : LightShadow{false, false},
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
            glGenTextures(1, &shadow_map_);

            glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map_);

            for (unsigned int i = 0; i < 6; ++i)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_map_, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ShadowSetup(true);
        }

        void PointLight::DrawShadows(const graphics::Shader& shader)
        {            
            if(!ShadowSetup())
                SetupShadows();
            
            glViewport(0, 0,  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            auto projection = clutch::Perspective((90.0f * clutch::PI) / 180.0f, (float)constants::SHADOW_WIDTH / (float)constants::SHADOW_HEIGHT, 0.5f, max_distance_);

            shader.SetUniform4f("light_pos", position_);
            std::vector<clutch::Mat4<float>> transforms{};
            transforms.push_back(projection * FaceTransform({ 1.0f, 0.0f, 0.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({-1.0f, 0.0f, 0.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f,-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f,-1.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f, 0.0f, 1.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));
            transforms.push_back(projection * FaceTransform({ 0.0f, 0.0f,-1.0f, 0.0f}, {0.0f,-1.0f, 0.0f, 0.0f}));

             for(int i = 0; i < 6; i++)
                shader.SetUniformMat4("face_vp[" + std::to_string(i) + "]", transforms[i]);    
            
            shader.SetUniformFloat("far_plane", max_distance_);
            
        }

        void PointLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("light_pos",   position_);
            shader.SetUniform4f("light_color", color_);
            shader.SetUniformFloat("max_distance", max_distance_);
            shader.SetUniformInt("cast_shadow", shadows_);

            glActiveTexture(GL_TEXTURE12);
            shader.SetUniformInt("shadow_map", 12);
            glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map_);
        }

        void PointLight::Erase()
        {

        }

        void PointLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
