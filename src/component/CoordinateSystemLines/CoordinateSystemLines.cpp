#include "CoordinateSystemLines.h"
#include "../Transform/Transform.h"
#include "../../object/Object/Object.h"

namespace CMP 
{
    CoordinateSystemLines::CoordinateSystemLines(OBJ::Object* game_object, glm::vec3 min, glm::vec3 max, glm::vec3 interval, glm::vec3 color): Component(Kind::COORDINATESYSTEMLINES, game_object)
    {
        this->game_object = game_object;
        this->min = min;
        this->max = max;
        this->interval = interval;
        this->color = color;
        init();
    }

    CoordinateSystemLines::~CoordinateSystemLines()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete this->vertices;
    }

    void CoordinateSystemLines::start()
    {

    }

    void CoordinateSystemLines::update()
    {

    }

    void CoordinateSystemLines::render(MAT::ShaderProgram* shader)
    {
        shader->use();

        glBindVertexArray(VAO);
        if (changeFlag) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_num * sizeof(float), vertices, GL_STATIC_DRAW);
            int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);

            shader->setUniformValue("color", glm::value_ptr(color), 3);

            Transform* transform = (Transform*)game_object->getComponent(CMP::Kind::TRANSFORM);
            if (transform == NULL)
            {
                std::cout << "coordinate system component depends on transform" << std::endl;
                return;
            }
            glm::mat4 model_mat = transform->model_mat;
            shader->setUniformMatrixValue("model", glm::value_ptr(model_mat), 4);

            changeFlag = false;
        }

        glDrawArrays(GL_LINES, 0, vertices_num / 3);
        glBindVertexArray(0);
    }

    void CoordinateSystemLines::init()
    {
        glm::vec3 line_num = (max - min) / interval;
        vertices_num = unsigned int(line_num.x + line_num.y + line_num.z + 0.00001f) * 4 * 3;
        vertices = new float[vertices_num]; 

        //x
        unsigned int offset = 0;
        for (int i = 0; i < line_num.x; i++)
        {
            vertices[offset + 12 * i + 0] = min.x + i * interval.x;
            vertices[offset + 12 * i + 1] = min.y;
            vertices[offset + 12 * i + 2] = min.z;

            vertices[offset + 12 * i + 3] = min.x + i * interval.x;
            vertices[offset + 12 * i + 4] = max.y;
            vertices[offset + 12 * i + 5] = min.z;

            vertices[offset + 12 * i + 6] = min.x + i * interval.x;
            vertices[offset + 12 * i + 7] = min.y;
            vertices[offset + 12 * i + 8] = min.z;

            vertices[offset + 12 * i + 9] = min.x + i * interval.x;
            vertices[offset + 12 * i + 10] = min.y;
            vertices[offset + 12 * i + 11] = max.z;
        }

        //y
        offset += unsigned int(line_num.x + 0.00001f) * 4 * 3;
        for (int i = 0; i < line_num.y; i++)
        {
            vertices[offset + 12 * i + 0] = min.x;
            vertices[offset + 12 * i + 1] = min.y + i * interval.y;
            vertices[offset + 12 * i + 2] = min.z;

            vertices[offset + 12 * i + 3] = max.x;
            vertices[offset + 12 * i + 4] = min.y + i * interval.y;
            vertices[offset + 12 * i + 5] = min.z;

            vertices[offset + 12 * i + 6] = min.x;
            vertices[offset + 12 * i + 7] = min.y + i * interval.y;
            vertices[offset + 12 * i + 8] = min.z;

            vertices[offset + 12 * i + 9] = min.x;
            vertices[offset + 12 * i + 10] = min.y + i * interval.y;
            vertices[offset + 12 * i + 11] = max.z;
        }

        //z
        offset += unsigned int(line_num.y + 0.00001f) * 4 * 3;
        for (int i = 0; i < line_num.z; i++)
        {
            vertices[offset + 12 * i + 0] = min.x;
            vertices[offset + 12 * i + 1] = min.y;
            vertices[offset + 12 * i + 2] = min.z + i * interval.z;

            vertices[offset + 12 * i + 3] = max.x;
            vertices[offset + 12 * i + 4] = min.y;
            vertices[offset + 12 * i + 5] = min.z + i * interval.z;

            vertices[offset + 12 * i + 6] = min.x;
            vertices[offset + 12 * i + 7] = min.y;
            vertices[offset + 12 * i + 8] = min.z + i * interval.z;

            vertices[offset + 12 * i + 9] = min.x;
            vertices[offset + 12 * i + 10] = max.y;
            vertices[offset + 12 * i + 11] = min.z + i * interval.z;
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
}