#include "Slider.h"

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
    Slider::Slider(OBJ::Object* game_object, std::string slider_img_path, glm::vec2 slider_position, glm::vec2 slider_scale, std::string node_img_path, glm::vec2 node_start, glm::vec2 node_end, glm::vec2 node_scale, float slider_depth, float node_depth, glm::vec3 color, float init_value, glm::vec2 interval, void* onclick(void*)): Label(game_object, slider_img_path, slider_position, slider_scale, slider_depth, color)
    {
        this->onclick = onclick;
        this->click_state = false;

        float ratio = (init_value - interval.x) / (interval.y - interval.x);

        glm::vec2 node_postion = ratio * (node_end - node_start) + node_start;
        node_label = new Label(NULL, node_img_path, node_postion, node_scale, node_depth, color);
        this->node_start = node_start;
        this->node_end = node_end;
        this->interval = interval;
        this->onclick = onclick;
    }

    Slider::~Slider()
    {

    }

    void Slider::update()
    {
        testClick();
    }

    void Slider::render(MAT::ShaderProgram* shader)
     {
        Label::render(shader);
        node_label->render(shader);
    }

    void Slider::testClick()
    {
        
        if (!INPUT::left_button_click)
        {
            click_state = false;
            return;
        }

        glm::vec2 delta = glm::abs(position - glm::vec2(INPUT::mouse_x, INPUT::mouse_y));
        if (delta.x < scale.x / 2 && delta.y < scale.y / 2 && INPUT::mouse_x >= node_start.x && INPUT::mouse_x <= node_end.x)
        {
            if (click_state)
            {
                float ratio = (INPUT::mouse_x - node_start.x) / (node_end.x - node_start.x);
                float value = ratio * (interval.y - interval.x) + interval.x;
                onclick(&value);
                glm::vec2 node_postion = ratio * (node_end - node_start) + node_start;
                node_label->setPosition(node_postion);
            }
            else
            {
                click_state = true;
            }
        }
        else
        {
            click_state = false;
        }
        return;
    }
}