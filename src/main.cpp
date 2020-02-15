#define  GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <g_manager.hpp>
#include <transforms.hpp>
#include <controller.hpp>

int main(int argc, char **argv)
{
    nitro::graphics::Manager manager{};
    
    nitro::core::PointLight light{};
    nitro::core::Actor      cyborg{"cyborg/cyborg.obj"};
    
    cyborg.Translate(clutch::Translation(0.0f, -2.5f, 0.0f));
    light.Translate(clutch::Translation(0.0f, -10.0f, 0.0f));
    
    nitro::core::Scene scene{};
    nitro::input::Controller controller1{scene.CameraPtr()};

    scene.AddActor(cyborg);
    scene.AddLight(light);

    controller1.AddButton(GLFW_KEY_W, scene.CameraPtr(), &nitro::core::Camera::Forward);
    controller1.AddButton(GLFW_KEY_A, scene.CameraPtr(), &nitro::core::Camera::Left);
    controller1.AddButton(GLFW_KEY_S, scene.CameraPtr(), &nitro::core::Camera::Backward);
    controller1.AddButton(GLFW_KEY_D, scene.CameraPtr(), &nitro::core::Camera::Right);

    float current_time = 0.0f;
    float delta_time   = 0.0f;
    float last_frame   = 0.0f;

    while(!glfwWindowShouldClose(manager.get_window()->get_window_ptr()))
    { 
        current_time = glfwGetTime();
        delta_time =  current_time - last_frame;
        last_frame = current_time;

        controller1.HandleInput(*manager.get_window(),delta_time);

        manager.UpdateScene(scene);

        //glfwPollEvents();
        if (glfwGetKey(manager.get_window()->get_window_ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(manager.get_window()->get_window_ptr(), GL_TRUE);
    }

    glfwTerminate();
    return 0;
}