#include "Camera.h"
#include "../../object/Object/Object.h"
#include "../Transform/Transform.h"
#include <GLFW/glfw3.h>

namespace SCREEN
{
    extern int screen_width;
    extern int screen_height;
    extern GLFWwindow* window;
} // namespace SCREEN

namespace CMP
{
    Camera::Camera(
        OBJ::Object *game_object, 
        glm::vec3 position, glm::vec3 front, glm::vec3 up,
        float fov, float near_plane, float far_plane,
        float sensitivity, float speed
    ): Component(Kind::CAMERA, game_object)
    {
        this->position = position;
        this->front = front;
        this->up = up;
        this->fov = fov;
        this->near_plane = near_plane;
        this->far_plane = far_plane;
        this->mode = Mode::MODE_3D;

        this->view_mat = glm::mat4(1.0f);
        this->projection_mat = glm::mat4(1.0f);

        this->freeze = false;

        //direction
        last_x = float(SCREEN::screen_width) / 2.0f;
        last_y = float(SCREEN::screen_height) / 2.0f;
        first_mouse = true;
        pitch = glm::degrees(glm::asin(front.y));
        yaw = glm::degrees(glm::acos(front.x / cos(glm::radians(pitch)))) - 180;
        this->sensitivity = sensitivity;

        //postion
        this->speed = speed;
    }
    
    Camera::~Camera()
    {
    }

    void Camera::start()
    {

    }

    void Camera::update()
    {
        updatePosition();
        updateDirection();
        updateScroll();
        
        if(changeFlag)
        {
            view_mat = glm::lookAt(position, position + front, up);
            projection_mat = glm::perspective(glm::radians(fov), float(SCREEN::screen_width) / float(SCREEN::screen_height), near_plane, far_plane);
            /*if(this->game_object->parent != NULL)
            {
                glm::mat4 parent_mat = ((Transform *)(this->game_object->parent->getComponent(CMP::Kind::TRANSFORM)))->model_mat;
                view_mat = parent_mat * view_mat;
            }*/
            changeFlag = false;
        }
    }

    void Camera::setFreezeFlag(bool flag)
    {
        freeze = flag;
    }

    void Camera::updateDirection()
    {
        if (mode == Mode::MODE_2D)
        {
            return;
        }
        if(INPUT::mouse_x == 0 && INPUT::mouse_y == 0)
        {
            return;
        }
        
        if(first_mouse)
        {
            last_x = INPUT::mouse_x;
            last_y = INPUT::mouse_y;
            first_mouse = false;
            return;
        }
        float xoffset = INPUT::mouse_x - last_x;
        float yoffset = last_y - INPUT::mouse_y;
        
        last_x = INPUT::mouse_x;
        last_y = INPUT::mouse_y;

        if (xoffset == 0 && yoffset == 0 || freeze)
        {
            return;
        }
        changeFlag = true;
        
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
    }

    void Camera::updateScroll()
    {
        if (freeze)
        {
            return;
        }

        if (mode == Mode::MODE_2D)
        {
            return;
        }
        if(fov >= 1.0f && fov <= 60.0f)
            fov -= INPUT::yoffset;
        if(fov <= 1.0f)
            fov = 1.0f;
        if(fov >= 90.0f)
            fov = 90.0f;
        INPUT::yoffset = 0;
        changeFlag = true;
    }

    void Camera::updatePosition()
    {
        if (freeze)
        {
            return;
        }
        float distance = speed * TIME::delta_time;
        if (mode == Mode::MODE_2D)
        {
            if (glfwGetKey(SCREEN::window, GLFW_KEY_W) == GLFW_PRESS)
                position += up * distance;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_S) == GLFW_PRESS)
                position -= up * distance;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_A) == GLFW_PRESS)
                position -= glm::normalize(glm::cross(front, up)) * distance;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_D) == GLFW_PRESS)
                position += glm::normalize(glm::cross(front, up)) * distance;
        }
        else if (mode == Mode::MODE_3D)
        {
            if (glfwGetKey(SCREEN::window, GLFW_KEY_W) == GLFW_PRESS)
                position += distance * front;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_S) == GLFW_PRESS)
                position -= distance * front;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_A) == GLFW_PRESS)
                position -= glm::normalize(glm::cross(front, up)) * distance;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_D) == GLFW_PRESS)
                position += glm::normalize(glm::cross(front, up)) * distance;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                position -= up * distance;
            if (glfwGetKey(SCREEN::window, GLFW_KEY_SPACE) == GLFW_PRESS)
                position += up * distance;
        }
        changeFlag = true;
    }

    void Camera::set(PropertyType property_type, glm::vec3 value, OBJ::Object *object)
    {
        this->changeFlag = true;
        switch (property_type)
        {
        case POSTION:
            this->position = value;
            break;
        case FRONT:
            this->front = value;
            break;
        case UP:
            this->up = value;
            break;
        default:
            this->changeFlag = false;
            break;
        }
    }

    void Camera::set(PropertyType property_type, float value, OBJ::Object *object)
    {
        this->changeFlag = true;
        switch (property_type)
        {
        case FOV:
            this->fov = value;
            break;
        case NEAR_PLANE:
            this->near_plane = value;
            break;
        case FAR_PLANE:
            this->far_plane = value;
            break;
        default:
            this->changeFlag = false;
            break;
        }
    }

    void Camera::switchMode(Mode mode)
    {
        this->mode = mode;
        this->changeFlag = true;
    }

    void Camera::apply(OBJ::Prefab *prefab)
    {
        delete ((Camera *)(*(prefab->components))[kind]);
        (*(prefab->components))[kind] = (Camera *)CMP::Component::createComponent(this);
        (*(prefab->components))[kind]->game_object = NULL;
    }
} // namespace CMP
