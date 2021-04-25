#include "Mesh.h"

namespace MOD
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MAT::Texture>& textures, MAT::ShaderProgram* shader_program, GLenum mesh_type)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        this->shader_program = shader_program;
        this->mesh_type = mesh_type;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    void Mesh::render()
    {
        // bind appropriate textures
        /*unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;*/
        for (int i = 0; i < textures.size() && i < 16; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            //string number;
            //string name = textures[i].type;
            //if (name == "texture_diffuse")
            //    number = std::to_string(diffuseNr++);
            //else if (name == "texture_specular")
            //    number = std::to_string(specularNr++); // transfer unsigned int to stream
            //else if (name == "texture_normal")
            //    number = std::to_string(normalNr++); // transfer unsigned int to stream
            //else if (name == "texture_height")
            //    number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            shader_program->setUniformValue((std::string("texture") + std::to_string(i)).c_str(), &i, 1);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].texture_id);
        }

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], mesh_type);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], mesh_type);

        // set the vertex attribute pointers
        // vertex Positions
        int position_layout = shader_program->layouts->at(MAT::ShaderProgram::LayoutType::POSTION_L);
        glEnableVertexAttribArray(position_layout);
        glVertexAttribPointer(position_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        int normal_layout = shader_program->layouts->at(MAT::ShaderProgram::LayoutType::NORMAL_L);
        glEnableVertexAttribArray(normal_layout);
        glVertexAttribPointer(normal_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        int texture_layout = shader_program->layouts->at(MAT::ShaderProgram::LayoutType::TEXTURE_L);
        glEnableVertexAttribArray(texture_layout);
        glVertexAttribPointer(texture_layout, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        int tangent_layout = shader_program->layouts->at(MAT::ShaderProgram::LayoutType::TANGENT_L);
        glEnableVertexAttribArray(tangent_layout);
        glVertexAttribPointer(tangent_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        int bitangent_layout = shader_program->layouts->at(MAT::ShaderProgram::LayoutType::BITANGENT_L);
        glEnableVertexAttribArray(bitangent_layout);
        glVertexAttribPointer(bitangent_layout, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        glBindVertexArray(0);
    }
}