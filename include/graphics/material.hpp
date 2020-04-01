#ifndef MATERIAL_H
#define MATERIAL_H

#include <vec3.hpp>

namespace nitro
{
    namespace graphics
    {
        struct Material
        {
            clutch::Vec3<float> diffuse;
            clutch::Vec3<float> specular;
            clutch::Vec3<float> ambient;
            float shininess;
        };
        
    }
}

#endif