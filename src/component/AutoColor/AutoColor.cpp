#include "AutoColor.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace SCREEN
{
	extern GLFWwindow* window;
} // namespace SCREEN

namespace CMP
{
	AutoColor::AutoColor(OBJ::Object* game_object, glm::vec4 color, glm::vec4 background) : Component(Kind::AUTOCOLOR, game_object)
	{
		this->color = color;
		this->background = background;
		this->init_color = color;
		this->init_background = background;
		this->mode = Mode::STATIC;
		this->cycle = 5;
		this->last_change_time = 0;
		this->send_flag = 1;
		this->use_position_flag = 0;
		this->time = 0.0f;
	}

	AutoColor::~AutoColor()
	{
	}

	void AutoColor::start()
	{

	}

	void AutoColor::update()
	{
		changeMode();
		switch (mode)
		{
		case Mode::STATIC:
			staticMode();
			break;
		case Mode::PLUSE:
			pluseMode();
			break;
		case Mode::FLASH:
			flushMode();
			break;
		case Mode::COLOR_CYCLE:
			colorCycleMode();
			break;
		case Mode::NEON:
			neonMode();
			break;
		default:
			break;
		}

		time += TIME::delta_time;
		if (time > cycle)
		{
			time = 0.0f;
			last_change_time = 0.0f;
		}
	}

	void AutoColor::changeMode()
	{
		if (time - last_change_time < 0.3f)
		{
			return;
		}

		bool changed = false;
		if (glfwGetKey(SCREEN::window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			int mode_index = int(mode) + 1;
			if (mode_index > int(Mode::NEON))
			{
				mode = Mode::STATIC;
			}
			else
			{
				mode = Mode(mode_index);
			}
			changed = true;
		}
		if (glfwGetKey(SCREEN::window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			int mode_index = int(mode) - 1;
			if (int(mode_index) < int(Mode::STATIC))
			{
				mode = Mode::COLOR_CYCLE;
			}
			else
			{
				mode = Mode(mode_index);
			}
			changed = true;
		}
		if (glfwGetKey(SCREEN::window, GLFW_KEY_1) == GLFW_PRESS)
		{
			mode = Mode::PLUSE;
			init_color = glm::vec4(1, 1, 1, 1);
			changed = true;
		}
		if (glfwGetKey(SCREEN::window, GLFW_KEY_2) == GLFW_PRESS)
		{
			mode = Mode::PLUSE;
			init_color = glm::vec4(0, 1, 0, 1);
			changed = true;
		}
		if (glfwGetKey(SCREEN::window, GLFW_KEY_3) == GLFW_PRESS)
		{
			mode = Mode::STATIC;
			init_color = glm::vec4(0, 0, 1, 1);
			changed = true;
		}

		if (changed)
		{
			use_position_flag = 0;
			send_flag = 1;
			switch (mode)
			{
			case Mode::STATIC:
				std::cout << "static mode" << std::endl;
				break;
			case Mode::PLUSE:
				std::cout << "pluse mode" << std::endl;
				break;
			case Mode::FLASH:
				std::cout << "flash mode" << std::endl;
				break;
			case Mode::COLOR_CYCLE:
				std::cout << "color cycle mode" << std::endl;
				break;
			case Mode::NEON:
				std::cout << "neon mode" << std::endl;
				use_position_flag = 1;
				break;
			default:
				break;
			}
			last_change_time = time;
			color = init_color;
			background = init_background;
		}
	}

	void AutoColor::changeBackground()
	{
		float brightness = 0.3f * color.r + 0.6f * color.g + 0.1f * color.b;
		if (brightness >= 0.5)
		{
			background = glm::vec4(0, 0, 0, 1);
		}
		else
		{
			float sum_color = color.r + color.g + color.b;
			float b_ratio = sum_color / (color.b + 0.00001f);
			float r_ratio = sum_color / (color.r + 0.00001f);
			float g_ratio = sum_color / (color.g + 0.00001f);
			float back_sum = b_ratio + r_ratio + g_ratio;
			float back_r = r_ratio / back_sum * 3 < 1 ? r_ratio / back_sum * 3 : 1;
			float back_g = g_ratio / back_sum * 3 < 1 ? g_ratio / back_sum * 3 : 1;
			float back_b = b_ratio / back_sum * 3 < 1 ? b_ratio / back_sum * 3 : 1;
			background = glm::vec4(back_r, back_g, back_b, 1);
		}
	}

	void AutoColor::pluseMode()
	{
		if (time > (cycle / 2))
		{
			float ratio = (cycle - time) / (cycle / 2);
			color = init_color * glm::sin(ratio * glm::pi<float>() / 2);
		}
		else
		{
			float ratio = time / (cycle / 2);
			color = init_color * glm::sin(ratio * glm::pi<float>() / 2);
		}
		changeBackground();
	}

	void AutoColor::staticMode()
	{
		changeBackground();
	}

	void AutoColor::flushMode()
	{
		if (time > cycle / 50 && time < cycle / 2 || time > cycle / 2 + cycle / 50 && time < cycle)
		{
			color = init_color;
		}
		else
		{
			color = glm::vec4(0, 0, 0, 1);
		}
	}

	void AutoColor::colorCycleMode()
	{
		float otoc = cycle / 3;//One_third_of_cycle
		if (time < otoc) //(1,0,0,1) -> (0,1,0,1)
		{
			color = glm::vec4(1 - time / otoc, time / otoc, 0, 1);
		}
		else if (time >= otoc && time < 2 * otoc)//(0,1,0,1) -> (0,0,1,1)
		{
			color = glm::vec4(0, 1 - (time - otoc) / otoc, (time - otoc) / otoc, 1);
		}
		else if (time >= 2 * otoc)
		{
			color = glm::vec4((time - 2 * otoc) / otoc, 0, 1 - (time - 2 * otoc) / otoc, 1);
		}
	}

	void AutoColor::neonMode()
	{

	}
}  