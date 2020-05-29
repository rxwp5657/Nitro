#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <mesh.hpp>
#include <texture.hpp>
#include <drawable.hpp>
#include <shader.hpp>
#include <vec3.hpp>
#include <vec2.hpp>

namespace nitro
{
    namespace graphics
    {
        class Model : public Drawable
        {
        public:

            Model(const std::string& name);
            Model(const std::vector<Mesh>& meshes);

            void FlipUV();

            void Draw(const Shader& shader, bool default_framebuffer = true) override;
            void Erase() override;

            void Color(float r, float g, float b);

        private:
            std::vector<Mesh> meshes_;
            std::string directory_;

            void Setup(const Shader& shader) override;
            
            void LoadModel(const std::string& name);
            void ProcessNode(aiNode *node, const aiScene *scene, std::vector<Texture>& loaded_textures);
            Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene, std::vector<Texture>& loaded_textures);
            Material LoadMaterials(aiMaterial *mat);
            std::vector<Texture> LoadTextures(aiMaterial *mat, 
                                              aiTextureType type, 
                                              std::string typeName,
                                              std::vector<Texture>& loaded_textures);
            

            template <typename T, typename U>
            void ProcessEntry(T& vec_a, U& vec_b)
            {
                vec_a.x = vec_b.x;
                vec_a.y = vec_b.y;
                vec_a.z = vec_b.z;
            }

            template <typename T, typename U>
            void ProcessColor(T& vec_a, U& vec_b)
            {
                vec_a.x = vec_b.r;
                vec_a.y = vec_b.g;
                vec_a.z = vec_b.b;
            }
        };
    }
}

#endif