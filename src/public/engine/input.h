#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

namespace INPUT
{
    float mouse_x = 0;
    float mouse_y = 0;
    float mouse_move_x = 0;
    float mouse_move_y = 0;
    bool first_mouse = true;
    float xoffset = 0;
    float yoffset = 0;
    bool left_button_click = false;
    bool right_button_click = false;

    void mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        /*std::cout << action << std::endl;*/
        if (action == GLFW_PRESS)
        {
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                left_button_click = true;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                left_button_click = true;
                break;
            default:
                return;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                left_button_click = false;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                left_button_click = false;
                break;
            default:
                return;
            }
        }
        return;
    }

    void mouseMoveCallback(GLFWwindow* window, double x, double y)
    {
        mouse_move_x = float(x) - mouse_x;
        mouse_move_y = mouse_y - float(y);
        if (mouse_move_x <= 2 && mouse_move_x >= -2)
        {
            mouse_move_x = 0;
        }
        if (mouse_move_y <= 2 && mouse_move_y >= -2)
        {
            mouse_move_y = 0;
        }
        mouse_x = float(x);
        mouse_y = float(y);
    }

    void scrollCallback(GLFWwindow* window, double x, double y)
    {
        xoffset = float(x);
        yoffset = float(y);
    }
} // namespace INPUT


#endif