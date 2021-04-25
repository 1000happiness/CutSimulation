#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./public/engine/screen.h"
#include "./public/engine/input.h"
#include "./public/engine/time.h"
#include "./public/engine/shader.h"

#include "./public/scene/scene.h"

GLFWwindow* init(int screenWidth, int screenHeight);
void processInput(GLFWwindow *window);

OBJ::EmptyObject* root_object;
SceneType scene_type;

int main()
{
    SCREEN::window = init(SCREEN::screen_width, SCREEN::screen_height);
    
    if (SCREEN::window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    scene_type = SceneType::WORKPIECE;
    root_object = makeScene(scene_type);

    root_object->start();

    while (!glfwWindowShouldClose(SCREEN::window))
    {
        TIME::updateTime(float(glfwGetTime()));

        processInput(SCREEN::window);

        root_object->update();
        root_object->render();

        glfwSwapBuffers(SCREEN::window);
        glfwPollEvents();
    }
    //end
    glfwTerminate();

    return 0;
}

GLFWwindow* init(int screen_width, int screen_height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "main", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, screen_width, screen_height);

    return window;
}

void processInput(GLFWwindow *window)
{
    static float last_time = 0;
    float now = float(glfwGetTime());
    if (now - last_time < 0.2f)
    {
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        last_time = now;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        root_object = switchUpScene(scene_type);
        last_time = now;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        root_object = switchDownScene(scene_type);
        last_time = now;
    }
}