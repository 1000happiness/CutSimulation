#ifndef SCREEN_H
#define SCREEN_H

#include <GLFW/glfw3.h>

namespace SCREEN
{
    int STANDARD_SIZE = 600;
    int screen_width = 1600;
    int screen_height = 900;
    GLFWwindow* window = NULL;

    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        screen_width = width;
        screen_height = height;
    }
} // namespace SCREEN


#endif