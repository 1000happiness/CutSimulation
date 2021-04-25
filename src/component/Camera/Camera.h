
#ifndef CMP_CAMERA_H
#define CMP_CAMERA_H

#include <string>

#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Component/Component.h"

namespace INPUT
{
    extern float mouse_x;
    extern float mouse_y;
    extern bool first_mouse;
    extern float xoffset;
    extern float yoffset;
    extern float mouse_move_x;
    extern float mouse_move_y;
}

namespace TIME
{
    extern float delta_time;
} // namespace TIME




namespace CMP
{
    class Camera : public Component
    {
    public: 
        enum PropertyType
        {
            POSTION,
            FRONT,
            UP,
            FOV,
            NEAR_PLANE,
            FAR_PLANE
        };

        enum class Mode
        {
            MODE_2D,
            MODE_3D
        };
    private:
        bool freeze;

        //direction
        float last_x;
        float last_y;
        bool first_mouse;
        float yaw;
        float pitch;
        float sensitivity;
        
        //position
        float speed;

        void updateDirection();
        void updateScroll();
        void updatePosition();

        Mode mode;
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        float fov;
        float near_plane;
        float far_plane;
        glm::mat4 view_mat;
        glm::mat4 projection_mat;

        Camera(
            OBJ::Object* game_object = NULL,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float fov = 60.0f, float near_plane = 0.01f, float far_plane = 20.0f, float sensitivity = 0.02f, float speed = 5.0f
        );
        ~Camera();

        void start();
        void update();

        void set(PropertyType property_type, glm::vec3 value, OBJ::Object *object);
        void set(PropertyType property_type, float value, OBJ::Object *object);

        void switchMode(Mode mode);
        void setFreezeFlag(bool flag);

        void apply(OBJ::Prefab *prefab);
    };
} // namespace CMP

#endif