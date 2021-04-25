#ifndef CMP_BUTTON_H
#define CMP_BUTTON_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/ext.hpp>

#include "../../material/Texture/Texture.h"
#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../UI/Label.h"
#include "../Component/Component.h"

namespace CMP
{
	class Button : public Label
	{
	public:
		Button(OBJ::Object* game_object = NULL, std::string img_path = "", glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(0.0f), float depth=0.0f, glm::vec3 color = glm::vec3(1.0f), void *onclick(void*) = NULL, std::string click_value = "");
		~Button();

		void update();
	private:
		void testClick();
		void* (*onclick)(void*);

		std::string click_value;
		bool click_state;
		bool color_change_flag;
	};
}

#endif // !CMP_BUTTON_H

