#include "CameraObject.h"
#include "../../component/Camera/Camera.h"
#include "../../component/Transform/Transform.h"

namespace OBJ
{
    CameraObject::CameraObject(unsigned int object_id, const std::string uniform_buffer_name, CMP::Camera *camera): Object(object_id, CAMERA)
    {
        glGenBuffers(1, &uniform_buffer_object);
        glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_object);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        GLuint binding_point = MAT::shader_controller->getBufferBindingIndex(uniform_buffer_name);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, uniform_buffer_object);

        this->components->insert(std::make_pair(CMP::Kind::CAMERA, camera)); 
        camera->game_object = this;
    }

    CameraObject::~CameraObject()
    {
        if(isPrefab && this->object_id == prefab->object->object_id)
        {
            return;
        }
    }

    void CameraObject::render()
    {
        if (!this->isActive)
        {
            return;
        }

        renderChilds();

        glm::mat4 view_mat = ((CMP::Camera *)this->components->at(CMP::Kind::CAMERA))->view_mat;
        glm::mat4 projection_mat = ((CMP::Camera *)this->components->at(CMP::Kind::CAMERA))->projection_mat;

        glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_object);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection_mat));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view_mat));
        CMP::Camera* camera = (CMP::Camera*)getComponent(CMP::Kind::CAMERA);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), glm::value_ptr(camera->position));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
} // namespace OBJ
