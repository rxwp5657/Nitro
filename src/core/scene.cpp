#include <scene.hpp>

namespace nitro
{
    namespace core
    {
        Scene::Scene(const std::vector<std::shared_ptr<Actor>>      actors,
                     const std::vector<std::shared_ptr<PointLight>> lights,
                     const Camera& camera)
        : actors_{actors},
          point_lights_{lights},
          camera_{camera}
        {
            glGenBuffers(1, &light_block_buffer_);
        }

        Scene::Scene()
        : actors_{},
          point_lights_{},
          camera_{}
        {
            glGenBuffers(1, &light_block_buffer_);
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

        void Scene::AddLight(const std::shared_ptr<PointLight> light)
        {
            point_lights_.push_back(light);
        }

        void Scene::LoadLights() const
        {
            const unsigned long num_lights = point_lights_.size();
            PointLight lights[num_lights];
            
            glBindBuffer(GL_UNIFORM_BUFFER, light_block_buffer_);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * num_lights, NULL, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, light_block_buffer_);

            for(int i = 0; i < num_lights; i++)
            {
                lights[i] = *point_lights_[i];
            }

            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lights), lights);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        Camera Scene::SceneCamera() const
        {
            return camera_;
        }

        Camera* Scene::CameraPtr()
        {
            return &camera_;
        }
    }
}