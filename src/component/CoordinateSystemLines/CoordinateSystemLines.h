#ifndef CMP_COORDINATESYSTEMLINES_H
#define CMP_COORDINATESYSTEMLINES_H

#include <glm/vec3.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "../../component/Component/Component.h"

namespace CMP
{
	class CoordinateSystemLines : public Component
	{
	public:
		CoordinateSystemLines(OBJ::Object* game_object = NULL, glm::vec3 min = glm::vec3(1.0f), glm::vec3 max = glm::vec3(1.0f), glm::vec3 interval = glm::vec3(1.0f), glm::vec3 color = glm::vec3(1.0f));
		~CoordinateSystemLines();

		void start();
		void update();
		void render(MAT::ShaderProgram* shader);
	private:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int vertices_num;
		float* vertices;
		glm::vec3 min;
		glm::vec3 max;
		glm::vec3 interval;
		glm::vec3 color;

		void init();
	};
}

#endif 
