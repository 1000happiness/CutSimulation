#ifndef AUTOROTATION_H
#define AUTOROTATION_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Component/Component.h"

namespace CMP
{
	class AutoRotation: public Component
	{
	public:
		enum PropertyType
		{
			ANGLE,
			AXIS,
			ROTATION_FLAG,
		};

		float angle;//degree
		glm::vec3 axis;
		bool rotation_flag;

		AutoRotation(OBJ::Object* game_object = NULL, float angle = 1.0f,glm::vec3 axis = glm::vec3(0, 1.0f, 0));
		~AutoRotation();

		void start();
		void update();

		void set(PropertyType property_type, glm::vec3 value);
		void set(PropertyType property_type, float value);
		void set(PropertyType property_type, bool value);
	private:
		
	};


}

#endif // !AUTOROTATION_H

