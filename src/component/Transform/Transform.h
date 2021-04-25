#ifndef CMP_TRANSFORM_H
#define CMP_TRANSFORM_H

#include <string>

#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../Component/Component.h"

namespace CMP
{
    class Transform : public Component
    {
    private:
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 model_mat;
        glm::mat4 rotation_mat;
        
        enum PropertyType
        {
            POSTION,
            ROTATION,
            ROTATION_MAT,
            SCALE
        };

        Transform(OBJ::Object *game_object = NULL, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
        ~Transform();

        void start();
        void update();

        void set(PropertyType property_type, glm::vec3 value);
        void set(PropertyType property_type, glm::mat4 value);
        void apply(OBJ::Prefab *prefab);
    };
} // namespace CMP

#endif