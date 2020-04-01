#include <plane.hpp>

namespace nitro
{
    namespace core
    {
        Plane::Plane(PlaneType plane_type)
        : Actor{GenerateModel(plane_type)}
        {

        }

        Plane::Plane()
        : Actor{GenerateModel(PlaneType::XY)}
        {

        }

        nitro::graphics::Model Plane::GenerateModel(PlaneType plane_type) const
        {
          clutch::Vec3<float> normal{0.0f, 0.0f, 0.0f};
          
          if(plane_type == PlaneType::XY)
            normal.z = -1.0f;
          else if(plane_type == PlaneType::XZ)
            normal.y = 1.0f;
          else
            normal.x = 1.0f;

          std::vector<nitro::graphics::Vertex> vertices{Vertices(plane_type, normal)};
          std::vector<unsigned int> indices{Indices()};
          std::vector<nitro::graphics::Texture> textures{};

            nitro::graphics::Material material{{1.0f, 0.2f, 1.0f},
                                               {0.9f, 0.9f, 0.9f}, 
                                               {0.1f, 0.1f, 0.1f},
                                               200.0f,
                                               0.0f,
                                               0.0f};

            nitro::graphics::Mesh mesh{vertices, indices, textures, material};

            return nitro::graphics::Model{{mesh}};
        }
        
        std::vector<nitro::graphics::Vertex> Plane::Vertices(PlaneType plane_type, const clutch::Vec3<float> normal) const
        {
          std::vector<nitro::graphics::Vertex> result{};
          std::vector<clutch::Vec2<float>> texture{{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};
          
          for(float x = 0.0f; x <= 1.0f; x++)
            for(float y = 0.0f; y <= 1.0f; y++)
              for(float z = 0.0f; z <= 1.0f; z++)
              {
                clutch::Vec3<float> point{x,y,z};

                if(fabs(clutch::Dot(normal,point) - 0.0) < 0.001)
                {
                  nitro::graphics::Vertex vertex{point,normal,texture[result.size()],{},{}};
                  result.push_back(vertex);
                }
              }

          return result;
        }

        std::vector<unsigned int> Plane::Indices() const
        {
          std::vector<unsigned int> result{0, 1, 2, 1, 2 , 3};
          return result;
        }

    }
}