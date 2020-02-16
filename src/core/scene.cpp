#include <scene.hpp>

namespace nitro
{
    namespace core
    {
        Scene::Scene(const std::vector<std::shared_ptr<Actor>> actors,
                     const Camera& camera)
        : actors_{actors},
          camera_{camera}
        {

        }

        Scene::Scene()
        : actors_{},
          camera_{}
        {

        }

        std::vector<std::shared_ptr<Actor>> Scene::Actors() const
        {
            return actors_;
        }
    
        void Scene::AddActor(const std::shared_ptr<Actor> actor)
        {
            actors_.push_back(actor);
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