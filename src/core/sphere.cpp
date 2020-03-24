#include <sphere.hpp>

namespace nitro
{
    namespace core
    {
        Sphere::Sphere(float radius, int stacks, int sectors)
        : Actor{GenerateModel(radius, stacks, sectors)},
          radius_{radius},
          stacks_{stacks},
          sectors_{sectors}
        {
        
        }

        Sphere::Sphere()
        : Actor{GenerateModel(1.0f, 40, 80)},
          radius_{1.0f},
          stacks_{80},
          sectors_{80}
        {

        }

        /*
            Implementation based on SongHo's algorithm:
            http://www.songho.ca/opengl/gl_sphere.html
        */

        std::vector<nitro::graphics::Vertex> Sphere::Vertices(float radius, int stacks, int sectors) const
        {
            /*
            theta = (2 * PI) * ( 2 * PI / sectors);
            phi   = (PI / 2) - (PI * (StackStep / stacks)); 

            x = (r * cos(phi)) cos(theta)
            y = (r * cos(phi)) cos(theta)
            x =  r * sin(phi) 
            */

           std::vector<nitro::graphics::Vertex>  result{};

           float x, y, z, r_cos;
           float nx, ny, nz;
           float u, v;

           float sector_step = 2 * M_PI / sectors;
           float stack_step  = M_PI / stacks;

           float theta = 0.0f;
           float phi   = 0.0f;

           for(int i = 0; i <= stacks; i++)
           {
               phi   = (M_PI / 2) - i * stack_step;
               r_cos = radius * cosf(phi);
               
               for(int j = 0; j <= sectors; j++)
               {    
                    theta = j  * sector_step;
                    x = r_cos  * cosf(theta);
                    y = r_cos  * sinf(theta);
                    z = radius * sinf(phi);

                    nx = x / radius;
                    ny = y / radius;
                    nz = z / radius;

                    u  = (float) i / sectors;
                    v  = (float) j / stacks;

                    nitro::graphics::Vertex current_vertex{{x,y,z}, {nx,ny,nz}, {u,v},{},{}};
                    result.push_back(current_vertex);
               }
           }
           return result;
        }

        std::vector<unsigned int> Sphere::Indices(
            const std::vector<nitro::graphics::Vertex>& vertices,
            int stacks, 
            int sectors) const
        {
            std::vector<unsigned int> result{};

            unsigned int v1, v2;

            for(int i = 0; i < stacks; i++)
            {
                v1 = i * (sectors + 1);
                v2 = v1 + sectors + 1;   

                for(int j = 0; j < sectors; j++, v1++, v2++)
                {
                    if(i != 0)
                    {
                        result.push_back(v1);
                        result.push_back(v2);
                        result.push_back(v1 + 1);
                    }

                    if(i != (stacks - 1))
                    {
                        result.push_back(v1 + 1);
                        result.push_back(v2);
                        result.push_back(v2 + 1);
                    }
                }
            }
            return result;
        }

        nitro::graphics::Model Sphere::GenerateModel(float radius, int stacks, int sectors) const
        {
            std::vector<nitro::graphics::Vertex> vertices{Vertices(radius,stacks,sectors)};
            std::vector<unsigned int> indices{Indices(vertices, stacks, sectors)};
            std::vector<nitro::graphics::Texture> textures{};

            nitro::graphics::Material material{{1.0f, 0.2f, 1.0f},
                                               {0.9f, 0.9f, 0.9f}, 
                                               {0.1f, 0.1f, 0.1f},
                                               200.0f};

            nitro::graphics::Mesh mesh{vertices, indices, textures, material};

            return nitro::graphics::Model{{mesh}};
        }
    }
}