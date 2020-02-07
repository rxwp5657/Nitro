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
            std::vector<Texture> textures_loaded{};

            std::string path = "../resources/models/" + name;
            
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
            {
                throw std::runtime_error("Unable to open model: " + std::string{importer.GetErrorString()});
            }

            ProcessNode(scene->mRootNode, scene, textures_loaded);
        }

        void Model::ProcessNode(aiNode *node, const aiScene *scene, std::vector<Texture>& textures_loaded)
        {

            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
                meshes_.push_back(ProcessMesh(mesh, scene, textures_loaded));			
            }
            
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene, textures_loaded);
            }
        }

        Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene, std::vector<Texture>& textures_loaded)
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
            std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures_loaded);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR, "texture_specular", textures_loaded);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = LoadTextures(material, aiTextureType_HEIGHT, "texture_normal", textures_loaded);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = LoadTextures(material, aiTextureType_AMBIENT, "texture_height", textures_loaded);
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            
            // return a mesh object created from the extracted mesh data
            return Mesh{vertices, indices, textures};
        }

        std::vector<Texture> Model::LoadTextures(aiMaterial *mat, 
                                                 aiTextureType type, 
                                                 std::string type_name,
                                                 std::vector<Texture>& textures_loaded)
        {
            std::vector<Texture> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;
                for(unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if(std::strcmp(textures_loaded[j].get_path().data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if(!skip)
                {
                    Texture texture{std::string{str.C_Str()}, 
                                    "../resources/models/" + directory_,
                                    type_name,};
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
            return textures;
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