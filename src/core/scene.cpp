#include <scene.hpp>

namespace nitro
{
    namespace core
    {
        Scene::Scene(const std::vector<Actor>& actors,
                     const std::vector<PointLight>& lights,
                     const Camera& camera)
        : actors_{actors},
          lights_{lights},
          camera_{camera}
        {

        }

        Scene::Scene()
        : actors_{},
          lights_{},
          camera_{}
        {

        }

        std::vector<Actor> Scene::Actors() const
        {
            return actors_;
        }

        std::vector<PointLight> Scene::Lights() const
        {
            return lights_;
        }

        void Scene::AddActor(const Actor& actor)
        {
            actors_.push_back(actor);
        }

        void Scene::AddLight(const PointLight& light)
        {
            lights_.push_back(light);
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