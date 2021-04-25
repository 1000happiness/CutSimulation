#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

#include "../../material/ShaderController/ShaderController.h"
#include "../Object/Object.h"

namespace MAT
{
    extern ShaderController *shader_controller;
}

namespace OBJ
{
    class CameraObject : public Object
    {
    private:
        std::string uniform_buffer_name;
        GLuint uniform_buffer_object;
    public:
        CameraObject(unsigned int object_id, const std::string uniform_buffer_name, CMP::Camera *camera);
        ~CameraObject();

        void render();
    };
} // namespace OBJ


#endif