#define  GLEW_STATIC

#include <iostream>
#include <string>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <transforms.hpp>

#include <g_manager.hpp>
#include <controller.hpp>
#include <sphere.hpp>
#include <cube.hpp>
#include <plane.hpp>
#include <skybox.hpp>

int main(int argc, char **argv)
{
    nitro::graphics::Manager manager{};
    manager.AddShader("skybox", nitro::graphics::Shader{"skybox.vert","skybox.frag"});
    
    std::shared_ptr<nitro::core::Actor>             cylinder{std::make_shared<nitro::core::Actor>("cylinder2/cylinder.obj")};
    std::shared_ptr<nitro::core::Actor>             sphere{std::make_shared<nitro::core::Sphere>(1.0f)};
    std::shared_ptr<nitro::core::Actor>             cube{std::make_shared<nitro::core::Cube>(1.0f)};
    std::shared_ptr<nitro::core::PointLight>        light{std::make_shared<nitro::core::PointLight>()};
    std::shared_ptr<nitro::core::SpotLight>         spot{std::make_shared<nitro::core::SpotLight>()};
    std::shared_ptr<nitro::core::DirectionalLight>  dir{std::make_shared<nitro::core::DirectionalLight>()};

    std::shared_ptr<nitro::core::Actor> wall  {std::make_shared<nitro::core::Plane>(nitro::core::PlaneType::XY)};
    std::shared_ptr<nitro::core::Actor> floor {std::make_shared<nitro::core::Plane>(nitro::core::PlaneType::XZ)};


    wall->Scale( 10, 10, 10);
    wall->Translate(5.0f, 0.0f, -2.0f);
    wall->Color(1.0f, 1.0f, 1.0f);
    wall->Rotate(0.0f, clutch::PI, 0.0f),
    
    floor->Scale(10, 10, 10);
    floor->Translate(-5.0f, 0.0f, -2.f);
    floor->Color(1.0f, 1.0f, 1.0f);

    nitro::core::Skybox skybox{"SwedishRoyalCastle", 
    {"posx.jpg",
     "negx.jpg",
     "posy.jpg",
     "negy.jpg",
     "posz.jpg",
     "negz.jpg"}};

    sphere->Translate(  0.0f, 1.0f, 0.0f);
    cube->Translate(   -3.0f, 0.5f, 0.0f);
    cylinder->Translate(3.0f, 0.0f, 0.0f);

    spot->MovePos(0.0f, 5.0f, 5.0f);
    spot->MoveDir(0.0f,-1.0f, 0.0f);
    dir->Move(    0.0f,-10.0f,-3.0f);
    light->Move(  0.0f, 3.5f, 2.0f);

    cylinder->FlipUV();

    nitro::core::Scene scene{};
    nitro::input::Controller controller1{scene.CameraPtr()};
    
    scene.AddActor(cylinder);
    scene.AddActor(sphere);
    scene.AddActor(cube);

    scene.AddActor(wall);
    scene.AddActor(floor);
    
    scene.AddSpotLight(spot);
    //scene.AddPointLight(light);
    //scene.AddDirectionalLight(dir);
    
    scene.AddSkyBox(skybox);

    //light->CastShadow(true);
    //dir->CastShadow(true);
    spot->CastShadow(true);

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
        last_frame =  current_time;

        controller1.HandleInput(*manager.get_window(),delta_time);
        manager.UpdateScene(scene);
        
        if (glfwGetKey(manager.get_window()->get_window_ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(manager.get_window()->get_window_ptr(), GL_TRUE);
    }

    glfwTerminate();
    return 0;
}