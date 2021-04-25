#include "Button.h"

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
    Button::Button(OBJ::Object* game_object, std::string img_path, glm::vec2 position, glm::vec2 scale, float depth, glm::vec3 color, void* onclick(void*), std::string click_value): Label(game_object, img_path, position, scale, depth, color)
    {
        this->onclick = onclick;
        this->click_value = click_value;
        this->click_state = false;
        this->color_change_flag = false;
    }

    Button::~Button()
    {

    }

    void Button::update()
    {
        testClick();
    }

    void Button::testClick()
    {
        
        if (!INPUT::left_button_click)
        {
            click_state = false;
            if (color_change_flag)
            {
                color = color / 0.8f;
                color_change_flag = false;
            }
            return;
        }

        glm::vec2 delta = glm::abs(position - glm::vec2(INPUT::mouse_x, INPUT::mouse_y));
        if (delta.x < scale.x / 2 && delta.y < scale.y / 2)
        {
            if (!click_state)
            {
                color = color * 0.8f;
                color_change_flag = true;
                onclick(&click_value);
            }
            click_state = true;
        }
        return;
    }
}