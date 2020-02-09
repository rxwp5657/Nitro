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
            Manager(const Context* context = Context::get_instance(),
                    const Window& window   = Window{800, 600, "Nitro", nitro::graphics::WindowType::WINDOWED, true}, 
                    const Shader& shader   = Shader{"nitro.vert", "nitro.frag"});
            
            ~Manager();

            const Window* get_window() const;
            void UpdateScene(core::Scene scene);
            
        private:
            std::unique_ptr<const Context> context_;
            Window window_;
            Shader active_program_;
            std::vector<Shader> shaders_;
        };
    }
}

#endif
