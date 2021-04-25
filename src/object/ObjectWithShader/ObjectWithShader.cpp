#include "ObjectWithShader.h"
#include "../../component/Transform/Transform.h"
#include "../../material/ShaderController/ShaderController.h"

namespace MAT
{
    extern ShaderController* shader_controller;
}

namespace OBJ
{
    ObjectWithShader::ObjectWithShader(unsigned object_id, std::string shader_name) : Object(object_id, WITHSHADER)
    {
        this->shader = MAT::shader_controller->getShader(shader_name);
    }

    ObjectWithShader::~ObjectWithShader()
    {
    }

    void ObjectWithShader::render()
    {
        if (!this->isActive)
        {
            return;
        }

        for (auto i = this->components->begin(); i != this->components->end(); i++)
        {
            i->second->render(shader);
        }

        renderChilds();
    }
} // namespace OBJ


