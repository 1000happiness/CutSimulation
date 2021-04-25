#ifndef OBJ_CANVAS_H
#define OBJ_CANVAS_H

#include <string>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../../material/Texture/Texture.h"
#include "../Object/Object.h"

namespace SCREEN
{
    extern int screen_width;
    extern int screen_height;
} // namespace SCREEN

namespace OBJ
{
    class Triangle;

    class CanvasObject : public Object
    {
    private:
        
        MAT::ShaderProgram* shaderProgram;
        void initVertices();
    public:
        struct vertex 
        {
            glm::vec3 position;
            glm::vec3 color;
        };

        struct Side
        {
            float x_min;
            float x_index;
            float delta_x;
            int y_max;
            int y_min;
            Side* next;
            Side(float x_index, float delta_x, int y_max, int y_min, float x_min, Side* next)
            {
                this->x_index = x_index;
                this->delta_x = delta_x;
                this->y_max = y_max;
                this->y_min = y_min;
                this->x_min = x_min;
                this->next = next;
            }
            void push_back(Side *side)
            {
                Side* side_ptr = this;
                while (side_ptr->next != NULL)
                {
                    side_ptr = side_ptr->next;
                }
                side_ptr->next = side;
            }
        };

        struct scanning_interval 
        {
            int min;
            int max;
            std::vector<std::vector<glm::vec2>> intervals;
            scanning_interval(int min, int max, std::vector<std::vector<glm::vec2>> intervals)
            {
                this->min = min;
                this->max = max;
                this->intervals = intervals;
            }
        };

        unsigned int* VAO;
        unsigned int* VBO;
        glm::vec3 background;
        vertex* vertices;
        bool* background_flag_buffer;
        float* z_buffer;
        glm::vec2 shape;
        glm::vec2 position;
        bool special_depth_test_flag;

        CanvasObject(unsigned int object_id, MAT::ShaderProgram* shaderProgram, glm::vec2 shape = glm::vec2(1920, 1080), glm::vec2 postion = glm::vec2(0, 0), bool special_depth_test_flag = false);
        CanvasObject(unsigned int object_id, Prefab* prefab);
        ~CanvasObject();

        void sendData2GPU();

        void render();

        static Side* getSide(glm::vec3 position1, glm::vec3 position2);

        void addTriangle(Triangle triangle);
        bool depth_test(float z, int x_index, int y_index);
    };

    class Triangle
    {
    private:
        std::vector<CanvasObject::Side*> getSides();
    public:
        CanvasObject::vertex vertices[3];

        Triangle(glm::vec3 position[3], glm::vec3 color[3]);
        ~Triangle();

        glm::vec3 getColor(glm::vec3 position);
        glm::vec3 getPosition(float x, float y);
        CanvasObject::vertex getVertex(float x, float y);

        CanvasObject::scanning_interval getScanningInterval(int canvas_min, int canvas_max);
    };

    
} // namespace OBJ

#endif