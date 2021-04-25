#ifndef CMP_SLIDER_H
#define CMP_SLIDER_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/ext.hpp>

#include "../../material/Texture/Texture.h"
#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../UI/Label.h"
#include "../Component/Component.h"

namespace CMP
{
	class Slider : public Label
	{
	public:
		Slider(OBJ::Object* game_object = NULL, std::string slider_img_path = "", glm::vec2 slider_position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(0.0f), std::string node_img_path = "", glm::vec2 node_start = glm::vec2(0.0f), glm::vec2 node_end = glm::vec2(0.0f), glm::vec2 node_scale = glm::vec2(0.0f), float slider_depth = 0.0f, float node_depth = 0.0f, glm::vec3 color = glm::vec3(1.0f), float init_value = 0.0f, glm::vec2 interval = glm::vec2(0.0f, 1.0f), void* onclick(void*) = NULL);
		~Slider();

		void update();
		void render(MAT::ShaderProgram* shader);
	private:
		void testClick();
		void* (*onclick)(void*);

		bool click_state;
		Label* node_label;
		glm::vec2 node_start;
		glm::vec2 node_end;
		glm::vec2 interval;
	};
}

#endif // !CMP_BUTTON_H

