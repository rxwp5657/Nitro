#ifndef SPOT_LIGHT
#define SPOT_LIGHT

#include <vec4.hpp>
#include <transforms.hpp>

namespace nitro 
{
    namespace core
    {
        class SpotLight
        {
        public:

            SpotLight();

            SpotLight(clutch::Vec4<float> position,
                      clutch::Vec4<float> direction,
                      clutch::Vec4<float> color = {1.0f,1.0f,1.0f,1.0f},
                      float cutoff  = cos((clutch::PI * 45.0f) / 180.0f),
                      float max_distance = 5.0f,
                      float umbra    = cos((clutch::PI * 17.5f) / 180.0f),
                      float penumbra = cos((clutch::PI * 12.5f) / 180.0f));

            void Umbra(float umbra);
            void Penumbra(float penumbra);
            void Distance(float distance);
            void Cutoff(float cutoff);

            void MovePos(float x, float y, float z);
            void MoveDir(float x, float y, float z);

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> direction_;
            clutch::Vec4<float> color_;
            float cutoff_;
            float max_distance_;
            float umbra_;
            float penumbra_;
        };
    }
}


#endif