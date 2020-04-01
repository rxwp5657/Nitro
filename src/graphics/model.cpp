#include <model.hpp>

namespace nitro
{
    namespace graphics
    {
        Model::Model(const std::string& name)
        : directory_{name.substr(0, name.find_last_of('/'))}
        {
            LoadModel(name);
        }

        Model::Model(const std::vector<Mesh>& meshes)
        : meshes_{meshes}
        {

        }

        void Model::LoadModel(const std::string& name)
        {
            std::vector<Texture> loaded_textures{};

            std::string path = "../resources/models/" + name;
            
            Assimp::Importer importer;

            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
            {
                throw std::runtime_error("Unable to open model: " + std::string{importer.GetErrorString()});
            }

            ProcessNode(scene->mRootNode, scene, loaded_textures);
        }

        void Model::ProcessNode(aiNode *node, const aiScene *scene, std::vector<Texture>& loaded_textures)
        {

            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
                meshes_.push_back(ProcessMesh(mesh, scene, loaded_textures));			
            }
            
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene, loaded_textures);
            }
        }

        Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene, std::vector<Texture>& loaded_textures)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices{};
            std::vector<Texture> textures;

            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                clutch::Vec3<float> position{};
                clutch::Vec3<float> normal{};
                clutch::Vec2<float> texture{0.0f, 0.0f};
                clutch::Vec3<float> tangent{};
                clutch::Vec3<float> bitangent{};

                ProcessEntry<clutch::Vec3<float>, aiVector3D>(position, mesh->mVertices[i]);
                ProcessEntry<clutch::Vec3<float>, aiVector3D>(normal, mesh->mNormals[i]);

                if(mesh->mTextureCoords[0])
                {
                    texture.x = mesh->mTextureCoords[0][i].x;
                    texture.y = mesh->mTextureCoords[0][i].y;
                }
                
                ProcessEntry<clutch::Vec3<float>, aiVector3D>(tangent,   mesh->mTangents[i]);
                ProcessEntry<clutch::Vec3<float>, aiVector3D>(bitangent, mesh->mBitangents[i]);

                vertices.push_back(Vertex{position,normal,texture,tangent,bitangent});
            }

            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 

            // 1. diffuse maps
            std::vector<Texture> diffuse_maps = LoadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", loaded_textures);
            textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
            // 2. specular maps
            std::vector<Texture> specular_maps = LoadTextures(material, aiTextureType_SPECULAR, "texture_specular", loaded_textures);
            textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
            // 3. normal maps
            std::vector<Texture> normal_maps = LoadTextures(material, aiTextureType_HEIGHT, "texture_normal", loaded_textures);
            textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
            // 4. height maps
            std::vector<Texture> height_maps = LoadTextures(material, aiTextureType_DISPLACEMENT, "texture_height", loaded_textures);
            textures.insert(textures.end(), height_maps.begin(), height_maps.end());

            Material mesh_material = LoadMaterials(material);

            // return a mesh object created from the extracted mesh data
            return Mesh{vertices, indices, textures, mesh_material};
        }

        std::vector<Texture> Model::LoadTextures(aiMaterial *mat, 
                                                 aiTextureType type, 
                                                 std::string type_name,
                                                 std::vector<Texture>& loaded_textures)
        {
            std::vector<Texture> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for(unsigned int j = 0; j < loaded_textures.size(); j++)
                {
                    if(std::strcmp(loaded_textures[j].get_path().data(), str.C_Str()) == 0)
                    {
                        textures.push_back(loaded_textures[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if(!skip)
                {
                    Texture texture{std::string{str.C_Str()}, 
                                    "../resources/models/" + directory_,
                                    type_name};
                    textures.push_back(texture);
                    loaded_textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
            return textures;
        }

        Material Model::LoadMaterials(aiMaterial *mat)
        {
            Material material{};
            aiColor3D color{0.f, 0.f, 0.f};

            mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            ProcessColor<clutch::Vec3<float>, aiColor3D>(material.diffuse,color);

            mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
            ProcessColor<clutch::Vec3<float>, aiColor3D>(material.specular,color);

            mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
            ProcessColor<clutch::Vec3<float>, aiColor3D>(material.ambient,color);

            mat->Get(AI_MATKEY_SHININESS, material.shininess);
            mat->Get(AI_MATKEY_REFLECTIVITY, material.reflectivity);
            mat->Get(AI_MATKEY_REFRACTI, material.refractivity);

            return material;
        }

        void Model::FlipUV()
        {
            for(auto& mesh : meshes_)
                mesh.FlipUV();
        }

        void Model::Draw(const Shader& shader)
        {
            for(auto& mesh : meshes_)
            {
                mesh.Draw(shader);
            }
        }

        void Model::Erase()
        {

        }

        void Model::Setup(const Shader& shader)
        {

        }
    }
}