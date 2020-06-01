#include "../../include/input/controller.hpp"

namespace nitro
{
    namespace input
    {
        Controller::Controller(core::Camera *camera)
        : buttons_{},
          camera_{camera},
          last_x_pos_{800.0f / 2.0f},
          last_y_pos_{600.0f / 2.0f},
          yaw_{-90.0f},
          pitch_{0.0f}
        {
        }

        void Controller::HandleInput(const graphics::Window& window, float time)
        {   
            double xpos, ypos;
            
            glfwPollEvents();

            glfwGetCursorPos(window.get_window_ptr(), &xpos, &ypos);
            MouseHandler(xpos, ypos);

            for(auto& button : buttons_)
            {
                if(glfwGetKey(window.get_window_ptr(), button.first) == GLFW_PRESS)
                {
                    auto event = KeyBoardEvent{button.first, time};
                    (button.second)->call(event);
                }
            }
        }

        void Controller::MouseHandler(double xpos, double ypos)
        {
            static bool first = true;

            if (first)
            {
                last_x_pos_ = xpos;
                last_y_pos_ = ypos;
                first = false;
            }

            float sensitivity = 0.1f;

            yaw_   += sensitivity * (xpos - last_x_pos_);
            pitch_ += sensitivity * (last_y_pos_ - ypos);

            last_x_pos_ = xpos;
            last_y_pos_ = ypos;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch_ > 89.0f)
                pitch_ = 89.0f;
            if (pitch_ < -89.0f)
                pitch_ = -89.0f;

            float yaw_r   = (clutch::PI * yaw_) / 180; 
            float pitch_r = (clutch::PI * pitch_) / 180;

            clutch::Vec4<float> front;

            front.x = cos(yaw_r) * cos(pitch_r);
            front.y = sin(pitch_r);
            front.z = sin(yaw_r) * cos(pitch_r);
            front = clutch::Normalize(front);
                    
            // front  = target - pos
            // target = front + pos

            camera_->Look(front);
        }

    }
}