#ifndef CMP_AUTOCONFIGURATION_H
#define CMP_AUTOCONFIGURATION_H

#include <iostream>


#include <glm/vec4.hpp>

#include "../Component/Component.h"

namespace CMP
{
	class AutoConfiguration: public Component
	{
	public:
		glm::vec4 background_color;
		unsigned int clear_flag;

		AutoConfiguration(OBJ::Object* game_object = NULL);
		~AutoConfiguration();

		void start();
		void update();
	private:

	};

	
}


#endif // !CMP_AUTOCONFIGURATION_H
