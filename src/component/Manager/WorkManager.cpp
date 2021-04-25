#include "WorkManager.h"
#include <GLFW/glfw3.h>
#include "../../object/Object/Object.h"
#include "../Camera/Camera.h"
#include "../Transform/Transform.h"
#include "../WorkPiece/WorkPiece.h"
#include "../ParticleSystem/ParticleSystem.h"
#include <glm/gtx/string_cast.hpp>

namespace TIME
{
	extern float current_time;
}

namespace INPUT
{
	extern float mouse_x;
	extern float mouse_y;
	extern bool left_button_click;
}

namespace SCREEN
{
	extern int screen_width;
	extern int screen_height;
	extern GLFWwindow* window;
}

namespace CMP
{
	WorkManager::WorkManager(OBJ::Object* game_object, OBJ::Object* work_piece_object, OBJ::Object* cutter_object, OBJ::Object* camera_object, OBJ::Object* particle_system_object) : Component(Kind::WORKMANAGER, game_object)
	{
		this->work_piece_object = work_piece_object;
		this->cutter_object = cutter_object;
		this->camera_object = camera_object;
		this->particle_system_object = particle_system_object;
		
		mode = Mode::RUN;

		init();

		/*CMP::WorkPiece *work_piece = (CMP::WorkPiece*)work_piece_object->getComponent(CMP::Kind::WORKPIECE);
		std::vector<glm::vec2> control_points;
		control_points.push_back(glm::vec2(0.0f, 0.2f));
		control_points.push_back(glm::vec2(1.0f, 0.5f));
		control_points.push_back(glm::vec2(2.0f, 0.3f));
		control_points.push_back(glm::vec2(3.0f, 0.2f));
		addConstraint(new BezierConstraint(std::make_pair(0.0f, work_piece->x_length), control_points), false);*/
	}

	WorkManager::~WorkManager()
	{
	}

	void WorkManager::start()
	{

	}

	void WorkManager::update()
	{
		switchMode();
		if (mode == Mode::CONFIGURE) {
			moveCutter();
		}
	}

	void WorkManager::reset(WorkPiece::PieceKind piece_kind, float length, float radius)
	{
		WorkPiece* work_piece = (WorkPiece*)work_piece_object->getComponent(Kind::WORKPIECE);
		work_piece->reset(piece_kind, length, radius, NULL);
		init();

		CMP::Transform* transform = (CMP::Transform*)cutter_object->getComponent(CMP::Kind::TRANSFORM);
		transform->set(CMP::Transform::POSTION, glm::vec3(3, 1, 0));
	}


	void WorkManager::addConstraint(Constraint* constraint, bool upper_flag)
	{
		WorkPiece* work_piece = (WorkPiece*)work_piece_object->getComponent(Kind::WORKPIECE);
		if (upper_flag)
		{
			upper_constraints.push_back(constraint);
			for (float x = constraint->x_interval.first; x <= constraint->x_interval.second; x+=work_piece->SLICE_WIDTH)
			{
				float y = constraint->calculateRadius(x + 0.0001);
				work_piece->setRadius(x + 0.0001, y, true);
				((SliceConstraint*)upper_constraints[0])->setPoints(x, y);
			}
		}
		else
		{
			lower_constraints.push_back(constraint);
		}
	}

	void WorkManager::init()
	{
		upper_constraints.clear();
		lower_constraints.clear();
		WorkPiece* work_piece = (WorkPiece*)work_piece_object->getComponent(Kind::WORKPIECE);
		upper_constraints.push_back(new SliceConstraint(std::make_pair(0.0f, work_piece->x_length), work_piece->radius, work_piece->SLICE_WIDTH));

		CMP::ParticleSystem* particle_system = (CMP::ParticleSystem*)particle_system_object->getComponent(CMP::Kind::PARTICLESYSTEM);
		switch (work_piece->piece_kind)
		{
		case WorkPiece::PieceKind::WOOD:
			particle_system->color = wood_particle_color;
			break;
		case WorkPiece::PieceKind::IRON:
			particle_system->color = iron_particle_color;
			break;
		default:
			break;
		}
	}

	void WorkManager::switchMode()
	{
		static float last_time = 0;
		float now = float(TIME::current_time);
		if (now - last_time < 0.2f)
		{
			return;
		}
		last_time = now;
		if (glfwGetKey(SCREEN::window, GLFW_KEY_E) == GLFW_PRESS)
		{
			CMP::Camera* camera = (CMP::Camera*)camera_object->getComponent(CMP::Kind::CAMERA);
			if (mode == Mode::RUN)
			{
				mode = Mode::CONFIGURE;
				glfwSetInputMode(SCREEN::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				camera->setFreezeFlag(true);
			}
			else if(mode == Mode::CONFIGURE){
				mode = Mode::RUN;
				glfwSetInputMode(SCREEN::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				camera->setFreezeFlag(false);
			}
		}
	}

	void WorkManager::moveCutter()
	{
		static bool click_state = false;
		if (INPUT::left_button_click) 
		{
			CMP::Transform* transform = (CMP::Transform*)cutter_object->getComponent(CMP::Kind::TRANSFORM);
			CMP::Camera* camera = (CMP::Camera*)camera_object->getComponent(CMP::Kind::CAMERA);
			CMP::ParticleSystem *particle_system = (CMP::ParticleSystem*)particle_system_object->getComponent(CMP::Kind::PARTICLESYSTEM);
			glm::vec4 cutter_render_postion = camera->projection_mat * (camera->view_mat * transform->model_mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			glm::vec2 cutter_location = glm::vec2(cutter_render_postion.x / cutter_render_postion.z, cutter_render_postion.y / cutter_render_postion.z);
			glm::vec2 mouse_location = glm::vec2(INPUT::mouse_x / SCREEN::screen_width * 2.0f - 1.0f, (-1.0f) * INPUT::mouse_y / SCREEN::screen_height * 2.0f + 1.0f);
			glm::vec2 delta = mouse_location - cutter_location;
			if (click_state)
			{
				float speed = 0.01f;
				int step = 0;
				while (step < 10)
				{
					CutState cut_state = cut(transform->position + glm::vec3(glm::normalize(delta), 0.0f) * speed);
					if (cut_state == CutState::CUT)
					{
						particle_system->start_create_particles();
						speed = 0.002f;
						transform->set(CMP::Transform::PropertyType::POSTION, transform->position + glm::vec3(glm::normalize(delta), 0.0f) * speed);
						break;
					}
					else if (cut_state == CutState::EMPTY)
					{
						particle_system->stop_create_particles();
						speed = 0.01f;
						transform->set(CMP::Transform::PropertyType::POSTION, transform->position + glm::vec3(glm::normalize(delta), 0.0f) * speed);
						break;
					}
					else if (cut_state == CutState::STOP)
					{
						delta.y += 0.02;
						step++;
					}
				}
			}
			else 
			{
				if (delta.x > 0.0f && delta.x < 0.1f && delta.y > 0.0f && delta.y < 0.2f)
				{
					click_state = true;
				}
			}
		} 
		else 
		{
			click_state = false;
		}
	}

	WorkManager::CutState WorkManager::cut(glm::vec3 cutter_position)
	{
		WorkPiece* work_piece = (WorkPiece*)work_piece_object->getComponent(Kind::WORKPIECE);

		if (cutter_position.x > work_piece->x_length || cutter_position.x < 0.0f || cutter_position.y > work_piece->radius) {
			return CutState::EMPTY;
		}

		float max = 100.0f;
		for (auto constraint = upper_constraints.begin(); constraint != upper_constraints.end(); constraint++)
		{
			if (cutter_position.x <= (*constraint)->x_interval.first || cutter_position.x >= (*constraint)->x_interval.second)
			{
				continue;
			}

			float temp = (*constraint)->calculateRadius(cutter_position.x);
			if (temp < max)
			{
				max = temp;
			}
		}

		if (cutter_position.y > max)
		{
			return CutState::EMPTY;
		}

		float min = 0.0f;
		for (auto constraint = lower_constraints.begin(); constraint != lower_constraints.end(); constraint++)
		{
			if (cutter_position.x <= (*constraint)->x_interval.first || cutter_position.x >= (*constraint)->x_interval.second)
			{
				continue;
			}

			float temp = (*constraint)->calculateRadius(cutter_position.x);
			if (temp > min)
			{
				min = temp;
			}
		}

		if (cutter_position.y < min)
		{
			return CutState::STOP;
		}

		bool cut_flag = work_piece->setRadius(cutter_position.x, cutter_position.y, false);
		if (cut_flag)
		{
			((SliceConstraint*)upper_constraints[0])->setPoints(cutter_position.x, cutter_position.y);
			return CutState::CUT;
		}
		else
		{
			return CutState::STOP;
		}
	}
}