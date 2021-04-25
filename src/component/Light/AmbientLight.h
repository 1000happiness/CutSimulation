#ifndef CMP_AMBIENTLIGHT_H
#define CMP_AMBIENTLIGHT_H

#include <glm/vec3.hpp>

#include "../../component/Component/Component.h"

namespace CMP
{
	class AmbientLight: public Component
	{
	public:
		glm::vec3 color;
		float intensity;

		AmbientLight(OBJ::Object* game_object = NULL, float intensity = 0.1f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		~AmbientLight();

		glm::vec3 getAmbient();

		void start();
		void update();
	private:

	};
}

#endif
