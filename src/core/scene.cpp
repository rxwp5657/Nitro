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
          camera_{camera},
          skybox_{skybox}
        {
            glGenBuffers(1, &light_buffer_);
            glGenBuffers(1, &num_lights_buffer_);
        }

        Scene::Scene()
        : actors_{},
          point_lights_{},
          spot_lights_{},
          dir_lights_{},
          camera_{},
          skybox_{}
        {
            glGenBuffers(1, &light_buffer_);
            glGenBuffers(1, &num_lights_buffer_);
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

        void Scene::LoadLights() const
        {
            int size = sizeof(PointLight) * POINT_LIGHTS + 
                       sizeof(SpotLight)  * SPOT_LIGHTS  + 
                       sizeof(DirectionalLight) * DIRECTIONAL_LIGHTS;

            int offset = 0;
            
            LoadNumLights();

            glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_);
            glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, light_buffer_);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            offset += LoadPointLights(offset);
            offset += LoadSpotLights(offset);
            offset += LoadDirectionalLights(offset);

        }

        void Scene::LoadNumLights() const
        {
            int num_point = point_lights_.size(); 
            int num_spot  = spot_lights_.size();
            int num_dir   = dir_lights_.size();

            glBindBuffer(GL_UNIFORM_BUFFER, num_lights_buffer_);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * 3, NULL, GL_DYNAMIC_DRAW);
            
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &num_point);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int),     sizeof(int), &num_spot);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int) * 2, sizeof(int), &num_dir);

            glBindBufferBase(GL_UNIFORM_BUFFER, 2, num_lights_buffer_);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        int Scene::LoadPointLights(int start_offset) const
        {
            const unsigned long num_lights = point_lights_.size();
            PointLight lights[POINT_LIGHTS]{};
            
            glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_);

            for(int i = 0; i < num_lights; i++)
                lights[i] = *point_lights_[i];

            glBufferSubData(GL_UNIFORM_BUFFER, start_offset, sizeof(lights), lights);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return sizeof(lights);
        }

        int Scene::LoadSpotLights(int start_offset)  const
        {
            const unsigned long num_lights = spot_lights_.size();
            SpotLight lights[SPOT_LIGHTS]{};
            
            glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_);
            
            for(int i = 0; i < num_lights; i++)
                lights[i] = *spot_lights_[i];

            glBufferSubData(GL_UNIFORM_BUFFER, start_offset, sizeof(lights), lights);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return sizeof(lights);
        }

        int Scene::LoadDirectionalLights(int start_offset) const
        {
            const unsigned long num_lights = dir_lights_.size();
            DirectionalLight lights[DIRECTIONAL_LIGHTS]{};
            
            glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_);
            
            for(int i = 0; i < num_lights; i++)
                lights[i] = *dir_lights_[i];

            glBufferSubData(GL_UNIFORM_BUFFER, start_offset, sizeof(lights), lights);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return sizeof(lights);
        }

        Camera Scene::SceneCamera() const
        {
            return camera_;
        }

        Camera* Scene::CameraPtr()
        {
            return &camera_;
        }

        void Scene::DrawActors(const std::map<std::string, graphics::Shader>& shaders)
        {
            for(auto& actor : actors_)
                for(const auto& shader_name : actor->Shaders())
                    if(shaders.find(shader_name) != shaders.end())
                    {
                        auto shader = shaders.at(shader_name);
                        shader.Use();
                        camera_.Draw(shader);
                        actor->Draw(shader);
                    }
        }
            
        void Scene::Setup() const
        {
            LoadLights();
        }

        void Scene::Draw(const std::map<std::string, graphics::Shader>& shaders)
        {
            Setup();
            DrawActors(shaders);
            DrawSkyBox(shaders.at("skybox"));
        }
    }
}