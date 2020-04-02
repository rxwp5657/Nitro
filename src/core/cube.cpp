#include <cube.hpp>

namespace nitro
{
    namespace core
    {
        Cube::Cube(float side)
        : Actor{GenerateModel(side)}, 
          side_{side}
        {

        }

        Cube::Cube()
        : Actor{GenerateModel(1.0f)},
          side_{1.0f}
        {

        }

        nitro::graphics::Model Cube::GenerateModel(float side) const
        {
            std::vector<nitro::graphics::Vertex> vertices{Vertices(side)};
            std::vector<unsigned int> indices{Indices()};
            std::vector<nitro::graphics::Texture> textures{};

            nitro::graphics::Material material{{1.0f,  0.2f, 1.0f},
                                               {0.9f,  0.9f, 0.9f}, 
                                               {0.1f,  0.1f, 0.1f},
                                               200.0f, 1.0f, 1.0f};

            nitro::graphics::Mesh mesh{vertices, indices, textures, material};

            return nitro::graphics::Model{{mesh}};
        }

        std::vector<nitro::graphics::Vertex> Cube::Vertices(float side) const
        {
            std::vector<nitro::graphics::Vertex> result{};

            side = side / 2;

            // All possible vertices
            clutch::Vec3<float> p1{side, side, side};
            clutch::Vec3<float> p2{-side, side, side};
            clutch::Vec3<float> p3{-side, -side, side};
            clutch::Vec3<float> p4{side, -side, side};
            clutch::Vec3<float> p5{side, side, -side};
            clutch::Vec3<float> p6{-side, side, -side};
            clutch::Vec3<float> p7{-side, -side, -side};
            clutch::Vec3<float> p8{side, -side, -side};

            // Face normals
            clutch::Vec3<float> n1{1.0f,  0.0f,  0.0f};
            clutch::Vec3<float> n2{0.0f,  1.0f,  0.0f};
            clutch::Vec3<float> n3{0.0f,  0.0f,  1.0f};
            clutch::Vec3<float> n4{-1.0f, 0.0f,  0.0f};
            clutch::Vec3<float> n5{0.0f, -1.0f,  0.0f};
            clutch::Vec3<float> n6{0.0f,  0.0f, -1.0f};

            // texture corrds
            clutch::Vec2<float> t1{0.0f, 0.0f};
            clutch::Vec2<float> t2{1.0f, 0.0f};
            clutch::Vec2<float> t3{0.0f, 1.0f};
            clutch::Vec2<float> t4{1.0f, 1.0f};
            
            //Front face
            result.push_back({p1,n3,t1,{},{}});
            result.push_back({p2,n3,t2,{},{}});
            result.push_back({p3,n3,t4,{},{}});
            result.push_back({p4,n3,t3,{},{}});

            // Top face
            result.push_back({p1,n2,t1,{},{}});
            result.push_back({p5,n2,t2,{},{}});
            result.push_back({p6,n3,t4,{},{}});
            result.push_back({p2,n2,t3,{},{}});
            
            // Left face
            result.push_back({p2,n4,t1,{},{}});
            result.push_back({p6,n4,t2,{},{}});
            result.push_back({p7,n4,t4,{},{}});
            result.push_back({p3,n4,t3,{},{}});
            
            // Right face
            result.push_back({p5,n1,t1,{},{}});
            result.push_back({p1,n1,t2,{},{}});
            result.push_back({p4,n1,t4,{},{}});
            result.push_back({p8,n1,t3,{},{}});
            
            // Back face
            result.push_back({p5,n6,t1,{},{}});
            result.push_back({p6,n6,t2,{},{}});
            result.push_back({p7,n6,t4,{},{}});
            result.push_back({p8,n6,t3,{},{}});

            // Bottom face
            result.push_back({p4,n5,t1,{},{}});
            result.push_back({p8,n5,t2,{},{}});
            result.push_back({p7,n5,t4,{},{}});
            result.push_back({p3,n5,t3,{},{}});
            
            return result;
        }

        std::vector<unsigned int> Cube::Indices() const
        {
            std::vector<unsigned int> result{};
            
            for(int i = 0; i < 24; i += 4)
            {
                // First triangle
                result.push_back(i);
                result.push_back(i+1);
                result.push_back(i+2);

                // Second triangle
                result.push_back(i);
                result.push_back(i+2);
                result.push_back(i+3);
            }

            return result;
        }
    }
}