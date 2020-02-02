#ifndef GMANAGER_H
#define GMANAGER_H

#include <memory>
#include <vector>
#include <context.hpp>
#include <window.hpp>
#include <shader.hpp>
#include <drawable.hpp>
#include <scene.hpp>

namespace nitro
{
    namespace graphics
    {
        class Manager
        {
        public:
            Manager(const Window& window, const Shader& shader);
            GLFWwindow* get_window() const;
            
            void UpdateScene(core::Scene scene);
            
        private:
            std::unique_ptr<Context> context_;
            Window window_;
            Shader active_program_;
            std::vector<Shader> shaders_;
        };
    }
}

#endif
