#include "Transform.h"
#include "../../object/Object/Object.h"

namespace CMP
{
    Transform::Transform(OBJ::Object *game_object, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale): Component(Kind::TRANSFORM, game_object)
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        this->model_mat = glm::mat4(1.0f);
        this->game_object = game_object;
        rotation_mat = glm::mat4(1.0f);
        rotation_mat = glm::rotate(rotation_mat, glm::radians(this->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation_mat = glm::rotate(rotation_mat, glm::radians(this->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        rotation_mat = glm::rotate(rotation_mat, glm::radians(this->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    
    Transform::~Transform()
    {
    }

    void Transform::start()
    {

    }

    void Transform::update()
    {
        if(changeFlag)
        {
            model_mat = glm::mat4(1.0f);
            model_mat = rotation_mat * model_mat;
            model_mat = glm::translate(model_mat, this->position);
            model_mat = glm::scale(model_mat, this->scale);
            
            if(this->game_object->parent != NULL)
            {
                glm::mat4 parent_mat = ((Transform *)(this->game_object->parent->getComponent(CMP::Kind::TRANSFORM)))->model_mat;
                model_mat = parent_mat * model_mat;
            }

            for (auto child = this->game_object->childs.begin(); child != this->game_object->childs.end(); child++) {
                Transform* transform = (Transform*)(*child)->getComponent(CMP::Kind::TRANSFORM);
                if (transform != NULL)
                {
                    transform->changeFlag = true;
                }
            }
            changeFlag = false;
        }
    }

    void Transform::set(PropertyType property_type, glm::vec3 value)
    {
        this->changeFlag = true;
        switch (property_type)
        {
        case POSTION:
            this->position = value;
            break;
        case ROTATION:
            this->rotation = value;
            rotation_mat = glm::mat4(1.0f);
            rotation_mat = glm::rotate(rotation_mat, glm::radians(this->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            rotation_mat = glm::rotate(rotation_mat, glm::radians(this->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation_mat = glm::rotate(rotation_mat, glm::radians(this->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
            break;
        case SCALE:
            this->scale = value;
            break;
        default:
            this->changeFlag = false;
            break;
        }
    }

    void Transform::set(PropertyType property_type, glm::mat4 value)
    {
        this->changeFlag = true;
        switch (property_type)
        {
        case ROTATION_MAT:
            this->rotation_mat = value;
            //TODO add convert to rotation
            break;
        default:
            this->changeFlag = false;
            break;
        }
    }

    void Transform::apply(OBJ::Prefab *prefab)
    {
        ((Transform *)(*(prefab->components))[kind])->set(Transform::SCALE, this->scale);
    }
} // namespace CMP
