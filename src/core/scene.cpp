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

        std::vector<Actor> Scene::Actors() const
        {
            return actors_;
        }

        std::vector<PointLight> Scene::Lights() const
        {
            return lights_;
        }

        Camera Scene::SceneCamera() const
        {
            return camera_;
        }
    }
}