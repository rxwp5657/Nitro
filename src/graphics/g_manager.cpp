#include <g_manager.hpp>

namespace nitro
{
    namespace graphics
    {
        Manager::Manager(const Context* context,
                         const Window& window, 
                         const Shader& shader)
        : context_{context},
          window_{window},
          shaders_{{"lighting", shader}}
        {
            for(const auto& shader: shaders_)
            {   
                if(shader.second.Status().status_code != nitro::utils::StatusCode::OK)
                {
                    throw std::runtime_error(shader.second.Status().message);
                }
            }
        }

        Manager::~Manager()
        {
            window_.Destroy();
            for(auto& shader : shaders_)
                shader.second.Delete();
        }

        const Window* Manager::get_window() const
        {
            return &window_;
        }

        void Manager::UpdateScene(core::Scene scene)
        {   
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
            
            for(auto& actor : scene.Actors())
            {
                for(const auto& shader_name : actor->Shaders())
                {
                    if(shaders_.find(shader_name) != shaders_.end())
                    {
                        auto shader = shaders_.at(shader_name);

                        shader.Use();
                        scene.LoadLights();
                        scene.SceneCamera().Draw(shader);
                        actor->Draw(shader);
                    }
                }
            }
            scene.DrawSkyBox(shaders_.at("skybox"));
            glfwSwapBuffers(window_.get_window_ptr());
        }

        void Manager::AddShader(const std::string& name, const Shader& shader)
        {
            if(shader.Status().status_code != nitro::utils::StatusCode::OK)
                throw std::runtime_error(shader.Status().message);
            
            shaders_[name] = shader;
        }
    }
}