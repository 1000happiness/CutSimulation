#include "Label.h"

namespace INPUT
{
    extern float mouse_x;
    extern float mouse_y;
}

namespace SCREEN
{
    extern int screen_width;
    extern int screen_height;
    
}

namespace CMP
{
    const float vertices_refercence[] =
    {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
    };

    Label::Label(OBJ::Object* game_object, std::string img_path, glm::vec2 position, glm::vec2 scale, float depth, glm::vec3 color):Component(Kind::LABEL, game_object)
    {
        MAT::Texture texture("texture0", img_path);
        textures.push_back(texture);

        this->position = position;
        this->scale = scale;

        standard_center = glm::vec2(position.x / SCREEN::screen_width * 2.0f - 1.0f, position.y / SCREEN::screen_height * (-2.0f) + 1.0f);
        standard_scale = glm::vec2(scale.x / SCREEN::screen_width, scale.y / SCREEN::screen_height) * 2.0f;

        this->depth = depth;

        this->color = color;

        this->first = true;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    Label::~Label()
    {

    }

    void Label::start()
    {

    }

    void Label::update()
    {

    }

    void Label::render(MAT::ShaderProgram* shader)
    {
        shader->use();
        if (first)
        {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices_refercence, GL_STATIC_DRAW);

            //position
            unsigned int pos_layout = 0;
            glVertexAttribPointer(pos_layout, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(pos_layout);
            //texture
            unsigned int tex_layout = 1;
            glVertexAttribPointer(tex_layout, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(tex_layout);
            first = false;
        }

        shader->linkTexture(textures);
        shader->setUniformValue("center", glm::value_ptr(standard_center), 2);
        shader->setUniformValue("scale", glm::value_ptr(standard_scale), 2);
        shader->setUniformValue("depth", &depth, 1);
        shader->setUniformValue("color", glm::value_ptr(color), 3);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Label::setPosition(glm::vec2 position)
    {
        this->position = position;

        standard_center = glm::vec2(position.x / SCREEN::screen_width * 2.0f - 1.0f, position.y / SCREEN::screen_height * (-2.0f) + 1.0f);
    }
}