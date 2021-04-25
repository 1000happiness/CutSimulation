#ifndef CUBE_H
#define CUBE_H

#include <string>

#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../../material/Texture/Texture.h"
#include "../Object/Object.h"

namespace OBJ
{
    class Cube : public Object
    {
    private:
        MAT::ShaderProgram *shaderProgram;
        void initVertices();
    public:
        static const unsigned int VERTICE_NUMBER;
        static const float pos_refercence[];
        static const float normal_reference[];
        static const float tex_refercence[];
        unsigned int *VAO;
        unsigned int *VBO;
        float *vertices;
        std::vector<MAT::Texture>* textures;
        GLenum data_RW_type;

        Cube(unsigned int object_id, MAT::ShaderProgram* shaderProgram, GLenum data_RW_type = GL_STATIC_DRAW);
        Cube(unsigned int object_id, MAT::ShaderProgram *shaderProgram, CMP::Transform *transform, GLenum data_RW_type = GL_STATIC_DRAW);
        Cube(unsigned int object_id, Prefab *prefab);
        ~Cube();

        void sendData2GPU();

        void render();
    };
} // namespace OBJ

#endif