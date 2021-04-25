#include "BezierPanel.h"
#include "../../object/Object/Object.h"
#include "../Manager/WorkManager.h"

namespace INPUT
{
    extern float mouse_x;
    extern float mouse_y;
    extern bool left_button_click;
}

namespace SCREEN
{
    extern int screen_width;
    extern int screen_height;

}

namespace CMP
{
    const float background_vertices_refercence[] =
    {
        -1.0f, -1.0f, 
         1.0f, -1.0f, 
         1.0f,  1.0f, 
         1.0f,  1.0f, 
        -1.0f,  1.0f, 
        -1.0f, -1.0f, 
    };

    const float cylinder_vertices_refercence[] =
    {
        -1.0f, -1.0f, 
        -1.0f,  1.0f, 
         1.0f,  1.0f, 
         1.0f, -1.0f, 
    };

    BezierPanel::BezierPanel(OBJ::Object* game_object, glm::vec2 position, glm::vec2 scale, float depth, OBJ::Object* work_manager_object, void* onclick(void*)) :Component(Kind::BEZIERPANEL, game_object)
    {
        CMP::WorkManager* work_manager = (CMP::WorkManager*)work_manager_object->getComponent(CMP::Kind::WORKMANAGER);

        upper_constraints = &work_manager->upper_constraints;
        lower_constraints = &work_manager->lower_constraints;

        if (upper_constraints != NULL)
        {
            this->interval = glm::vec2(upper_constraints->at(0)->x_interval.first, upper_constraints->at(0)->x_interval.second);
            this->height = upper_constraints->at(0)->calculateRadius(interval.x);
            this->upper_constraints = upper_constraints;
            for (auto constraint = upper_constraints->begin(); constraint != upper_constraints->end(); constraint++)
            {
                if ((*constraint)->x_interval.first < interval.x)
                {
                    interval.x = (*constraint)->x_interval.first;
                }

                if ((*constraint)->x_interval.second > interval.y)
                {
                    interval.y = (*constraint)->x_interval.second;
                }

                if ((*constraint)->calculateRadius((*constraint)->x_interval.first) > height)
                {
                    height = (*constraint)->calculateRadius((*constraint)->x_interval.first);
                }
            }
        }

        if (lower_constraints != NULL)
        {
            this->lower_constraints = lower_constraints;
            for (auto constraint = lower_constraints->begin(); constraint != lower_constraints->end(); constraint++)
            {
                if ((*constraint)->x_interval.first < interval.x)
                {
                    interval.x = (*constraint)->x_interval.first;
                }

                if ((*constraint)->x_interval.second > interval.y)
                {
                    interval.y = (*constraint)->x_interval.second;
                }

                if ((*constraint)->calculateRadius((*constraint)->x_interval.first) > height)
                {
                    height = (*constraint)->calculateRadius((*constraint)->x_interval.first);
                }
            }
        }

        this->position = position;
        this->scale = scale;

        standard_center = glm::vec2(position.x / SCREEN::screen_width * 2.0f - 1.0f, position.y / SCREEN::screen_height * (-2.0f) + 1.0f);
        standard_scale = glm::vec2(scale.x / SCREEN::screen_width, scale.y / SCREEN::screen_height) * 2.0f;

        this->depth = depth;
        this->onclick = onclick;

        this->temp_constraint = NULL;
        this->update_constraints_flag = false;
        this->x_offset = 0.02f;
        this->first = true;

        glGenVertexArrays(1, &background_VAO);
        glGenBuffers(1, &background_VBO);
        glGenVertexArrays(1, &cylinder_VAO);
        glGenBuffers(1, &cylinder_VBO);
        glGenVertexArrays(1, &constraints_VAO);
        glGenBuffers(1, &constraints_VBO);
    }

    BezierPanel::~BezierPanel()
    {

    }

    void BezierPanel::start()
    {

    }

    void BezierPanel::update()
    {
        testClick();
    }

    void BezierPanel::render(MAT::ShaderProgram* shader)
    {
        shader->use();
        if (first)
        {
            //background
            glBindVertexArray(background_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, background_VBO);
            glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), background_vertices_refercence, GL_STATIC_DRAW);
            //position
            unsigned int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);   

            //cylinder constraint
            glBindVertexArray(cylinder_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, cylinder_VBO);
            glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), cylinder_vertices_refercence, GL_STATIC_DRAW);
            //position
            pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);


            first = false;
        }
        glm::vec3 color = glm::vec3(1.0f);
        float standard_ratio = float(SCREEN::screen_height) / float(SCREEN::screen_width);
        shader->setUniformValue("standard_ratio", &standard_ratio, 1);
        shader->setUniformValue("center", glm::value_ptr(standard_center), 2);
        shader->setUniformValue("scale", glm::value_ptr(standard_scale), 2);
        shader->setUniformValue("depth", &depth, 1);

        bool background_flag = true;
        bool constraint_flag = false;
        //background
        shader->setUniformValue("color", glm::value_ptr(color), 3);
        shader->setUniformValue("background_flag", &background_flag, 1);
        shader->setUniformValue("constraint_flag", &constraint_flag, 1);
        glBindVertexArray(background_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);  
        glBindVertexArray(0);

        //cylinder
        background_flag = false;
        color = glm::vec3(0.0f);
        float width = interval.y - interval.x;
        float ratio = height / width;
          shader->setUniformValue("color", glm::value_ptr(color), 3);
        shader->setUniformValue("x_offset", &x_offset, 1);
        shader->setUniformValue("ratio", &ratio, 1);
        shader->setUniformValue("background_flag", &background_flag, 1);
        shader->setUniformValue("constraint_flag", &constraint_flag, 1);
        glBindVertexArray(cylinder_VAO);
        //glLineWidth(10.0f);    
        glDrawArrays(GL_LINE_STRIP, 0, 4); 
        glBindVertexArray(0);

        //constraint
        constraint_flag = true;
        shader->setUniformValue("background_flag", &background_flag, 1);
        shader->setUniformValue("constraint_flag", &constraint_flag, 1);
        glBindVertexArray(constraints_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, constraints_VBO);
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        shader->setUniformValue("color", glm::value_ptr(color), 3);
        for (auto constraint = upper_constraints->begin(); constraint != upper_constraints->end(); constraint++)
        {
            float* points_vertices = new float[(*constraint)->points.size() * 2];
            for (int i = 0; i < (*constraint)->points.size(); i++)
            {
                points_vertices[i * 2] = ((*constraint)->points[i].x - interval.x) / (interval.y - interval.x);//x
                points_vertices[i * 2 + 1] = (*constraint)->points[i].y / height;//y
            }

            glBufferData(GL_ARRAY_BUFFER, (*constraint)->points.size() * 2 * sizeof(float), points_vertices, GL_STATIC_DRAW);
            //position
            unsigned int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);

            //write
            glDrawArrays(GL_LINE_STRIP, 0, (*constraint)->points.size() - 1);

            delete [] points_vertices;
        }

        color = glm::vec3(0.0f, 1.0f, 0.0f);
        shader->setUniformValue("color", glm::value_ptr(color), 3);
        for (auto constraint = lower_constraints->begin(); constraint != lower_constraints->end(); constraint++)
        {
            float* points_vertices = new float[(*constraint)->points.size() * 2];
            for (int i = 0; i < (*constraint)->points.size(); i++)
            {
                points_vertices[i * 2] = ((*constraint)->points[i].x - interval.x) / (interval.y - interval.x);//x
                points_vertices[i * 2 + 1] = (*constraint)->points[i].y / height;//y
            }

            glBufferData(GL_ARRAY_BUFFER, (*constraint)->points.size() * 2 * sizeof(float), points_vertices, GL_STATIC_DRAW);
            //position
            unsigned int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);

            //write 
            glDrawArrays(GL_LINE_STRIP, 0, (*constraint)->points.size());

            delete [] points_vertices;
        }

        if (temp_constraint != NULL)
        {
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader->setUniformValue("color", glm::value_ptr(color), 3);
            float* points_vertices = new float[temp_constraint->points.size() * 2];
            for (int i = 0; i < temp_constraint->points.size(); i++)
            {
                points_vertices[i * 2] = (temp_constraint->points[i].x - interval.x) / (interval.y - interval.x);//x
                points_vertices[i * 2 + 1] = temp_constraint->points[i].y / height;//y
            }

            glBufferData(GL_ARRAY_BUFFER, temp_constraint->points.size() * 2 * sizeof(float), points_vertices, GL_STATIC_DRAW);
            //position
            unsigned int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);

            //write
            glDrawArrays(GL_LINE_STRIP, 0, temp_constraint->points.size());

            delete [] points_vertices;
        }
        
        //points
        if (!temp_points.empty())
        {
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader->setUniformValue("color", glm::value_ptr(color), 3);
            float* points_vertices = new float[temp_points.size() * 2];
            for (int i = 0; i < temp_points.size(); i++)
            {
                points_vertices[i * 2] = (temp_points[i].x - interval.x) / (interval.y - interval.x);//x
                points_vertices[i * 2 + 1] = temp_points[i].y / height;//y
            }

            glBufferData(GL_ARRAY_BUFFER, temp_points.size() * 2 * sizeof(float), points_vertices, GL_STATIC_DRAW);
            //position
            unsigned int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);

            //write
            glPointSize(3.0f);
            glDrawArrays(GL_POINTS, 0, temp_points.size());
            glPointSize(1.0f);

            delete[] points_vertices;
        }
        
        glBindVertexArray(0);
    }

    void BezierPanel::setTempConstraint(Constraint* constraint)
    {
        this->temp_constraint = constraint;
    }

    int BezierPanel::setTempPoints(int index, glm::vec2 point)
    {
        if (index == -1)
        {
            temp_points.push_back(point);
            return temp_points.size() - 1;
        }
        else
        {
            if (index < temp_points.size())
            {
                temp_points[index] = point;
                return index;
            }
        }
    }

    void BezierPanel::clearTempPoints()
    {
        temp_points.clear();
    }

    void BezierPanel::reset()
    {
        if (upper_constraints != NULL)
        {
            this->interval = glm::vec2(upper_constraints->at(0)->x_interval.first, upper_constraints->at(0)->x_interval.second);
            this->height = upper_constraints->at(0)->calculateRadius(interval.x);
            this->upper_constraints = upper_constraints;
            for (auto constraint = upper_constraints->begin(); constraint != upper_constraints->end(); constraint++)
            {
                if ((*constraint)->x_interval.first < interval.x)
                {
                    interval.x = (*constraint)->x_interval.first;
                }

                if ((*constraint)->x_interval.second > interval.y)
                {
                    interval.y = (*constraint)->x_interval.second;
                }

                if ((*constraint)->calculateRadius((*constraint)->x_interval.first) > height)
                {
                    height = (*constraint)->calculateRadius((*constraint)->x_interval.first);
                }
            }
        }

        if (lower_constraints != NULL)
        {
            this->lower_constraints = lower_constraints;
            for (auto constraint = lower_constraints->begin(); constraint != lower_constraints->end(); constraint++)
            {
                if ((*constraint)->x_interval.first < interval.x)
                {
                    interval.x = (*constraint)->x_interval.first;
                }

                if ((*constraint)->x_interval.second > interval.y)
                {
                    interval.y = (*constraint)->x_interval.second;
                }

                if ((*constraint)->calculateRadius((*constraint)->x_interval.first) > height)
                {
                    height = (*constraint)->calculateRadius((*constraint)->x_interval.first);
                }
            }
        }

        temp_points.clear();
        this->temp_constraint = NULL;
    }

    void BezierPanel::testClick()
    {

        if (!INPUT::left_button_click)
        {
            click_state = false;
            return;
        }

        glm::vec2 delta = glm::abs(position - glm::vec2(INPUT::mouse_x, INPUT::mouse_y));
        if (delta.x < scale.x / 2 && delta.y < scale.y / 2)
        {
            if (!click_state)
            {
                float width = interval.y - interval.x;
                float ratio = height / width;
                glm::vec2 begin(position.x - scale.x / 2 + x_offset * scale.x, position.y + scale.y / 2);
                glm::vec2 offset(INPUT::mouse_x - begin.x, begin.y - INPUT::mouse_y);
     
                glm::vec2 new_scale(scale.x * (1 - x_offset * 2), scale.x * (1 - x_offset * 2) * ratio);
      
                glm::vec2 click_value(offset.x / new_scale.x * width, offset.y / new_scale.y * height);
             
                onclick(&click_value);
            }
            click_state = true;
        }
        return;
    }
}