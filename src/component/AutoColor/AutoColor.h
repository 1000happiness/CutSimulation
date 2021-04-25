#ifndef AUTOCOLOR_H
#define AUTOCOLOR_H


#include <glm/vec4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../Component/Component.h"


namespace TIME
{
	extern float delta_time;
} // namespace TIME



namespace CMP
{
	class AutoColor :public Component
	{
	public:
		glm::vec4 color;
		glm::vec4 background;
		int use_position_flag;
		int send_flag;

		AutoColor(OBJ::Object* game_object = NULL, glm::vec4 color = glm::vec4(1, 1, 1, 1), glm::vec4 background = glm::vec4(0, 0, 0, 1));
		~AutoColor();
		void start();
		void update();
	private:
		enum class Mode
		{
			STATIC=1,
			PLUSE,
			FLASH,
			COLOR_CYCLE,
			NEON,
		};

		glm::vec4 init_color;
		glm::vec4 init_background;
		Mode mode;
		float time;
		float cycle;
		float last_change_time;

		void changeMode();
		void changeBackground();

		void flushMode();
		void pluseMode();
		void staticMode();
		void colorCycleMode();
		void neonMode();
	};

	
}

#endif // !AUTOCOLOR_H
