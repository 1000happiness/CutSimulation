#ifndef CMP_SPECULARLIGHT_H
#define CMP_SPECULARLIGHT_H

#include <glm/vec3.hpp>

#include "../../component/Component/Component.h"

namespace CMP
{
	class SpecularLight: public Component
	{
	public:
		glm::vec3 color;
		float intensity;

		SpecularLight(OBJ::Object* game_object = NULL, float intensity = 0.5f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		~SpecularLight();

		void start();
		void update();

		glm::vec3 getSpecular();
	private:

	};
}

#endif // !AMBIENT_LIGHT_H
