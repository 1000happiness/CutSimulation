#ifndef CMP_DIFFUSELIGHT_H
#define CMP_DIFFUSELIGHT_H

#include <glm/vec3.hpp>

#include "../../component/Component/Component.h"

namespace CMP
{
	class DiffuseLight: public Component
	{
	public:
		glm::vec3 color;
		float intensity;

		DiffuseLight(OBJ::Object* game_object = NULL, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		~DiffuseLight();

		glm::vec3 getDiffuse();

		void start();
		void update();
	private:

	};
}

#endif
