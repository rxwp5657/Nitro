#include <scene.hpp>

namespace nitro
{
    namespace core
    {
        Scene::Scene(const std::vector<std::shared_ptr<Actor>>      actors,
                     const std::vector<std::shared_ptr<PointLight>> point_lights,
                     const std::vector<std::shared_ptr<SpotLight>>  spot_lights,
                     const std::vector<std::shared_ptr<DirectionalLight>> dir_lights,
                     const Camera& camera,
                     const Skybox& skybox)
        : actors_{actors},
          point_lights_{point_lights},
          spot_lights_{spot_lights},
          dir_lights_{dir_lights},
          gbuffer_textures_{},
          camera_{camera},
          skybox_{skybox},
          gbuffer_{},
          shadow_buffer_{},
          update_VBO_{true}
        {

        }

        Scene::Scene()
        : actors_{},
          point_lights_{},
          spot_lights_{},
          dir_lights_{},
          gbuffer_textures_{},
          camera_{},
          skybox_{},
          gbuffer_{},
          shadow_buffer_{},
          update_VBO_{true}
        {

        }

        std::vector<std::shared_ptr<Actor>> Scene::Actors() const
        {
            return actors_;
        }

        std::vector<std::shared_ptr<PointLight>> Scene::PointLights() const
        {
            return point_lights_;
        }
    
        void Scene::AddActor(const std::shared_ptr<Actor> actor)
        {
            actors_.push_back(actor);
        }

        void Scene::AddPointLight(const std::shared_ptr<PointLight> light)
        {
            point_lights_.push_back(light);
        }

        void Scene::AddSpotLight(const std::shared_ptr<SpotLight> light)
        {
            spot_lights_.push_back(light);
        }

        void Scene::AddDirectionalLight(const std::shared_ptr<DirectionalLight> light)
        {
            dir_lights_.push_back(light);
        }

        void Scene::AddSkyBox(const Skybox& skybox)
        {
            skybox_ = skybox;
        }

        void Scene::DrawSkyBox(const graphics::Shader& shader)
        {
            shader.Use();
            camera_.Draw(shader);
            skybox_.Draw(shader);
        }

        Camera Scene::SceneCamera() const
        {
            return camera_;
        }

        Camera* Scene::CameraPtr()
        {
            return &camera_;
        }

        void Scene::EnableMultipass()
        {
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            glDepthFunc(GL_EQUAL);
        }

        void Scene::DisableMultipass()
        {
            glDisable(GL_BLEND);
            glDepthFunc(GL_LESS);
        }

        void Scene::SetDefaultViewPort(int width, int height)
        {
            glViewport(0,0,width,height); 
        }

        void Scene::ClearBuffers()
        {
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Scene::DrawActors(const graphics::Shader& shader, bool default_framebuffer)
        {
            for(const auto& actor : actors_)
                actor->Draw(shader, default_framebuffer);
        }

        void Scene::RenderShadows(const  std::map<std::string, graphics::Shader>& shaders)
        {   
            if(shaders.find("point_shadows") != shaders.end() && point_lights_.size() > 0)
                ForwardRenderShadows(shaders, point_lights_);

            if(shaders.find("spot_shadows") != shaders.end() && spot_lights_.size() > 0)
                ForwardRenderShadows(shaders, spot_lights_);
    

            if(shaders.find("directional_shadows") != shaders.end() && dir_lights_.size() > 0)
                ForwardRenderShadows(shaders, dir_lights_);
        }

        void Scene::ForwardRender(const  std::map<std::string, graphics::Shader>& shaders)
        {            
            if(shaders.find("point_lighting") != shaders.end() && point_lights_.size() > 0)
            {
                auto shader = shaders.at("point_lighting");
                ForwardRenderLights(shader, point_lights_);
            }

            if(shaders.find("directional_lighting") != shaders.end() && dir_lights_.size() > 0)
            {
                auto shader = shaders.at("directional_lighting");
                ForwardRenderLights(shader, dir_lights_);
            }

            if(shaders.find("spot_lighting") != shaders.end() && spot_lights_.size() > 0)
            {
                auto shader = shaders.at("spot_lighting");
                ForwardRenderLights(shader, spot_lights_);
            }
            
            DisableMultipass();
        }

        void Scene::DeferredRender(const std::map<std::string, graphics::Shader>& shaders)
        {

        }

        void Scene::Setup() const
        {
            
        }        
        
        void Scene::Draw(const std::map<std::string, graphics::Shader>& shaders, int screen_width, int screen_height)
        {
            //Setup();
            ClearBuffers();
            RenderShadows(shaders);
            
            SetDefaultViewPort(screen_width, screen_height);
            
            ClearBuffers();
            ForwardRender(shaders);

            DrawSkyBox(shaders.at("skybox"));
            skybox_.Unbind();
            
            /*
            auto debugger = utils::Debugger();
            debugger.DebugFrameBuffer(spot_lights_[0]->ShadowMap(0), shaders.at("debug"));
            */
        }
    }
}