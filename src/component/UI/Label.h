#ifndef CMP_LABEL_H
#define CMP_LABEL_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/ext.hpp>

#include "../../material/Texture/Texture.h"
#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../Component/Component.h"

namespace CMP
{
	class Label : public Component
	{
	public:
		Label(OBJ::Object* game_object = NULL, std::string img_path = "", glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(0.0f), float depth=0.0f, glm::vec3 color = glm::vec3(1.0f));
		~Label();

		void start();
		void update();
		void render(MAT::ShaderProgram *shader);

		void setPosition(glm::vec2 position);

		glm::vec3 color;
	protected:
		glm::vec2 position;
		glm::vec2 scale;
		glm::vec2 standard_center;
		glm::vec2 standard_scale;
		std::vector <MAT::Texture> textures;
		float depth;
		unsigned int VAO;
		unsigned int VBO;
		bool first;
	};
}

#endif // !CMP_LABEL_H

