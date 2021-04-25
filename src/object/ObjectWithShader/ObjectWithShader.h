#ifndef OBJ_OBJECTWITHSHADER_H
#define OBJ_OBJECTWITHSHADER_H

#include "../Object/Object.h"
#include "../../material/ShaderProgram/ShaderProgram.h"

namespace OBJ
{
    class ObjectWithShader : public Object
    {
    private:
        MAT::ShaderProgram* shader;
    public:
        ObjectWithShader(unsigned object_id, std::string shader_name);
        ~ObjectWithShader();
        void render();
    };
} // namespace OBJ

#endif

