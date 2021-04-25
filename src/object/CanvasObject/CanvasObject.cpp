#include "CanvasObject.h"
#include "../../component/Transform/Transform.h"

namespace OBJ
{
    CanvasObject::CanvasObject(unsigned int object_id, MAT::ShaderProgram* shaderProgram, glm::vec2 shape, glm::vec2 position, bool special_depth_test_flag) : Object(object_id, CANVAS)
    {
        this->shaderProgram = shaderProgram;
        this->position = position;
        this->shape = glm::floor(shape);
        this->z_buffer = new float[shape.y * shape.x];
        this->background_flag_buffer = new bool[shape.y * shape.x];
        this->special_depth_test_flag = special_depth_test_flag;
        for (int i = 0; i < int(shape.y * shape.x); i++)
        {
            this->z_buffer[i] = 0.0f;
            this->background_flag_buffer[i] = true;  
        }
        this->background = glm::vec3(1.0f, 1.0f, 1.0f);
        initVertices();
    }

    CanvasObject::CanvasObject(unsigned int object_id, Prefab* prefab) : Object(object_id, CANVAS)
    {
        this->VAO = ((CanvasObject*)prefab->object)->VAO;
        this->VBO = ((CanvasObject*)prefab->object)->VBO;
        this->vertices = ((CanvasObject*)prefab->object)->vertices;
        this->shaderProgram = ((CanvasObject*)prefab->object)->shaderProgram;
        this->isPrefab = true;
        this->prefab = prefab;
        prefab->used_objects->insert(this);
        for (auto i = prefab->components->begin(); i != prefab->components->end(); i++)
        {
            this->components->insert(std::make_pair(i->first, CMP::Component::createComponent(i->second)));
            this->components->at(i->first)->game_object = this;
        }
    }

    CanvasObject::~CanvasObject()
    {
        if (isPrefab && this->object_id == prefab->object->object_id)
        {
            return;
        }

        glDeleteVertexArrays(object_id, VAO);
        glDeleteBuffers(object_id, VBO);

        delete VAO;
        delete VBO;

        delete []vertices;
    }

    void CanvasObject::initVertices()
    {
        unsigned int vertice_size = POS_SIZE + COLOR_RGB_SIZE;
        unsigned int vertice_number = shape.y * shape.x;//shape.y是行数 shape.x是列数
        vertices = new vertex[vertice_number];

        float delta_x = 2.0f / SCREEN::screen_width;
        float delta_y = 2.0f / SCREEN::screen_height;
        float x_begin = ((-1) * shape.x / 2) * delta_x + position.x * delta_x;
        float y_begin = ((-1) * shape.y / 2) * delta_y + position.y * delta_y;
        for (unsigned int i = 0; i < vertice_number; i++)
        {
            if (i % int(shape.x) == 0 && i != 0)
            {
                x_begin = ((-1) * shape.x / 2) * delta_x + position.x * delta_x;
                y_begin += delta_y;
            }
            vertices[i].position = glm::vec3(x_begin, y_begin, 0);
            vertices[i].color = background;
            x_begin += delta_x;
        }

        /*for (int i = -shape.y / 2; i < shape.y / 2; i++)
        {
            int index = (i + shape.y / 2) * shape.x;
            std::cout << index << std::endl;
            for (int k = 0; k < 1000; k++)
            {
                vertices[index + k].color = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }*/

        this->VAO = new unsigned int;
        this->VBO = new unsigned int;
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
    }

    void CanvasObject::sendData2GPU()
    {
        shaderProgram->use();
        glBindVertexArray(*VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, (shape.x * shape.y * sizeof(vertex)), vertices, GL_STATIC_DRAW);

        unsigned int pos_layout = shaderProgram->layouts->at(MAT::ShaderProgram::LayoutType::POSTION_L);
        glVertexAttribPointer(pos_layout, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(pos_layout);

        unsigned int color_layout = shaderProgram->layouts->at(MAT::ShaderProgram::LayoutType::COLOR_L);
        glVertexAttribPointer(color_layout, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(color_layout);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void CanvasObject::render()
    {
        if (!this->isActive)
        {
            return;
        }
        renderChilds();

        shaderProgram->use();
        glBindVertexArray(*VAO);
        glPointSize(1.5f);
        glDrawArrays(GL_POINTS, 0, unsigned int(shape.x * shape.y)); 
        glBindVertexArray(0);
    }

    CanvasObject::Side* CanvasObject::getSide(glm::vec3 position1, glm::vec3 position2)
    {
        glm::vec3 position_y_max = position1;
        glm::vec3 position_y_min = position2;
        if (position2.y > position_y_max.y)
        {
            position_y_max = position2;
            position_y_min = position1;
        }
        int y_max = glm::floor(position_y_max.y);
        int y_min = glm::ceil(position_y_min.y);
        float delta_x = position_y_max.x == position_y_min.x ? 0: (position_y_max.x - position_y_min.x) / (position_y_max.y - position_y_min.y);
        float x_index = delta_x == 0 ? glm::max(position_y_min.x, position_y_max.x) : (y_min - position_y_min.y) / delta_x + position_y_min.x;
        float x_min = glm::min(position_y_min.x, position_y_max.x);
        return new Side(x_index, delta_x, y_max, y_min, x_min, NULL);
    }

    void CanvasObject::addTriangle(Triangle triangle)
    {
        scanning_interval intervals = triangle.getScanningInterval((-1) * shape.y / 2 + position.y, shape.y / 2 + position.y);
        for (int i = 0; i < intervals.max - intervals.min + 1; i++)
        {
            for (auto interval = intervals.intervals[i].begin(); interval != intervals.intervals[i].end(); interval++)
            {
                for (int k = glm::floor((*interval).x); k < glm::floor((*interval).y); k++)
                {
                    glm::vec3 vertex_position = triangle.getPosition(k, i + intervals.min);
                    glm::vec3 vertex_color = triangle.getColor(vertex_position);
                    unsigned int index = (i + intervals.min + shape.y / 2) * int(shape.x) + k + shape.x / 2;
                    if (background_flag_buffer[index])
                    {
                        vertices[index].color = vertex_color;
                        background_flag_buffer[index] = false;
                        z_buffer[index] = vertex_position.z;
                    }
                    else
                    {
                        if (depth_test(vertex_position.z, k + shape.x / 2, i + intervals.min + shape.y / 2))
                        {
                            vertices[index].color = vertex_color;
                            background_flag_buffer[index] = false;
                            z_buffer[index] = vertex_position.z;
                        }
                    }
                }
            }
        }
    }

    bool CanvasObject::depth_test(float z, int x_index, int y_index)
    {
        if (special_depth_test_flag)
        {
            float flag = glm::sin(2.0f * glm::pi<float>() / 320.0f * float(x_index - shape.x / 2)) * (-1.0f) * glm::sin(2.0f * glm::pi<float>() / 320.0f * float(y_index - shape.y / 2));
            if (flag >= 0)
            {
                return z > z_buffer[y_index * int(shape.x) + x_index];
            }
            else
            {
                return z < z_buffer[y_index * int(shape.x) + x_index];
            }
        }
        else
        {
            return z > z_buffer[y_index * int(shape.x) + x_index];
        }
    }

    Triangle::Triangle(glm::vec3 position[3], glm::vec3 color[3])
    {
        for (int i = 0; i < 3; i++)
        {
            vertices[i].position = position[i];
            vertices[i].color = color[i];
        }
    }

    Triangle::~Triangle()
    {

    }

    glm::vec3 Triangle::getColor(glm::vec3 position)
    {   
        float a1 = vertices[1].position.x - vertices[0].position.x + 0.0000001f, b1 = vertices[2].position.x - vertices[0].position.x + 0.0000001f, c1 = position.x - vertices[0].position.x + 0.0000001f;
        float a2 = vertices[1].position.y - vertices[0].position.y + 0.0000001f, b2 = vertices[2].position.y - vertices[0].position.y + 0.0000001f, c2 = position.y - vertices[0].position.y + 0.0000001f;
        float u = (b1 * c2 - b2 * c1) / (a2 * b1 - a1 * b2);
        float v = (c1 - a1 * u) / b1;
        glm::vec3 color = (1 - u - v) * vertices[0].color + u * vertices[1].color + v * vertices[2].color;
        return color;
    }

    glm::vec3 Triangle::getPosition(float x, float y)
    {
        glm::vec3 n = glm::cross(vertices[2].position - vertices[0].position, vertices[1].position - vertices[0].position);
        float z = (-1) * (n.x * (x - vertices[1].position.x) + n.y * (y - vertices[1].position.y)) / n.z + vertices[1].position.z;
        //std::cout << x << " " << y << " " << z << ":";
        return glm::vec3(x,y,z);
    }

    CanvasObject::vertex Triangle::getVertex(float x, float y)
    {
        float distance[3];
        float total = 0.0f;
        for (int i = 0; i < 3; i++)
        {
            distance[i] = glm::distance(glm::vec2(vertices[i].position.x, vertices[i].position.y), glm::vec2(x, y));
            if (distance[i] == 0)
            {
                distance[i] += 0.0000001f;
            }
            distance[i] = 1 / distance[i];
            total += distance[i];
        }
        CanvasObject::vertex vertex;
        vertex.position = glm::vec3(0.0f, 0.0f, 0.0f);
        vertex.color = glm::vec3(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 3; i++)
        {
            vertex.position += distance[i] / total * vertices[i].position;
            vertex.color += distance[i] / total * vertices[i].color;
            /*std::cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << std::endl;
            std::cout << vertex.color.x << " " << vertex.color.y << " " << vertex.color.z << std::endl;
            std::cout << distance[i] << std::endl;*/
        }
        return vertex;
    }


    CanvasObject::scanning_interval Triangle::getScanningInterval(int canvas_min, int canvas_max)
    {
        int min = int(glm::floor(vertices[0].position.y));
        int max = int(glm::ceil(vertices[0].position.y));
        for (int i = 1; i < 3; i++)
        {
            if (vertices[i].position.y < min)
            {
                min = int(glm::floor(vertices[i].position.y));
            }
            if (vertices[i].position.y > max)
            {
                max = int(glm::floor(vertices[i].position.y));
            }
        }
        if (min < canvas_min)
        {
            min = canvas_min;
        }
        if (max > canvas_max)
        {
            max = canvas_max;
        }

        std::vector<std::vector<glm::vec2>> intervals = std::vector<std::vector<glm::vec2>>(max - min + 1);
        for (int i = 0; i < max - min + 1; i++)
        {
            intervals[i] = std::vector<glm::vec2>();
        }

        //初始化活性边表
        std::vector<CanvasObject::Side*> active_side_table = std::vector<CanvasObject::Side*>(max - min + 1);
        for (int i = 0; i < max - min + 1; i++)
        {
            active_side_table[i] = NULL;
        }

        std::vector<CanvasObject::Side*> sides = getSides();
        for (auto item = sides.begin(); item != sides.end(); item++)
        {
            if ((*item)->y_max < min || (*item)->y_min > max)
            {
                continue;
            }

            if ((*item)->y_min < min)
            {
                (*item)->x_index = (min - (*item)->y_min) / (*item)->delta_x + (*item)->x_index;
                (*item)->y_min = min;
            }

            if ((*item)->y_max > max)
            {
                (*item)->x_index = (max - (*item)->y_max) / (*item)->delta_x + (*item)->x_index; 
                (*item)->y_max = max;
            }

            if (active_side_table[(*item)->y_min - min] == NULL)
            {
                 active_side_table[(*item)->y_min - min] = *item;
            }
            else
            {
                active_side_table[(*item)->y_min - min]->push_back((*item));
            }
        }
        
        //遍历活性边表
        for(int i = 0; i < max - min + 1; i++)
        {
            std::vector<float> nodes;
            CanvasObject::Side* side_ptr = active_side_table[i];
            while (side_ptr != NULL)
            {
                if (side_ptr->y_max == side_ptr->y_min)
                {
                    nodes.push_back(side_ptr->x_min);
                    nodes.push_back(side_ptr->x_index);
                    side_ptr = side_ptr->next;
                    continue;
                }
                nodes.push_back(side_ptr->x_index);
                if (min + i < side_ptr->y_max)
                {
                    if (i + 1 < max - min + 1)
                    {
                        if (active_side_table[i + 1] == NULL)
                        {
                            active_side_table[i + 1] = new CanvasObject::Side(side_ptr->x_index + side_ptr->delta_x, side_ptr->delta_x, side_ptr->y_max, side_ptr->y_min, side_ptr->x_min, NULL);
                        }
                        else
                        {
                            active_side_table[i + 1]->push_back(new CanvasObject::Side(side_ptr->x_index + side_ptr->delta_x, side_ptr->delta_x, side_ptr->y_max, side_ptr->y_min, side_ptr->x_min, NULL));
                        }
                    }
                }
                side_ptr = side_ptr->next;
            }
          
            std::sort(nodes.begin(), nodes.end());
            //nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
            for (int k = 0; k < nodes.size(); )
            {
                if (k + 1 < nodes.size())
                {
                    if (nodes[k] == nodes[k + 1])
                    {
                        intervals[i].push_back(glm::vec2(nodes[k], nodes[k]));
                        k += 1;
                    }
                    else
                    {
                        float x_min = glm::min(nodes[k], nodes[k + 1]);
                        float x_max = glm::max(nodes[k], nodes[k + 1]);
                        /*std::cout << "from  to: " << x_min << " " << x_max << std::endl;*/
                        intervals[i].push_back(glm::vec2(x_min, x_max));
                        k += 2;
                    }
                }
                else
                {
                    break;
                }
            }
        }

        return CanvasObject::scanning_interval(min, max, intervals);
    }

    std::vector<CanvasObject::Side*> Triangle::getSides()
    {
        std::vector<CanvasObject::Side*> sides = std::vector<CanvasObject::Side*>(3);
        sides[0] = CanvasObject::getSide(vertices[0].position, vertices[1].position);
        sides[1] = CanvasObject::getSide(vertices[1].position, vertices[2].position);
        sides[2] = CanvasObject::getSide(vertices[2].position, vertices[0].position);
        return sides;
    }
} // namespace OBJ
