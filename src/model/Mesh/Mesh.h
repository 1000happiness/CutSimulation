#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../../material/Texture/Texture.h"

namespace MOD
{
    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };

    class Mesh {
    public:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<MAT::Texture> textures;
        unsigned int VAO;

        // constructor
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MAT::Texture>& textures, MAT::ShaderProgram* shader_program, GLenum mesh_type = GL_STATIC_DRAW);

        // render the mesh
        void render();

    private:
        // render data 
        unsigned int VBO, EBO;
        MAT::ShaderProgram* shader_program;
        GLenum mesh_type;

        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}
#endif