#include "UIManager.h"
#include <GLFW/glfw3.h>
#include "../../object/Object/Object.h"
#include "../UI/Button.h"
#include "../Manager/WorkManager.h"
#include "../UI/BezierPanel.h"
#include "../UI/Text.h"

namespace CMP
{
	UIManager::UIManager(OBJ::Object* game_object,//base object 
		OBJ::Object* UI_iron_kind_button_object, OBJ::Object* UI_wood_kind_button_object, //kind select button
		std::vector<OBJ::Object*> UI_bezier_focus_buttons_objects, //bezier focus point select button
		OBJ::Object* bezier_pannel_object, //bezier panel
		OBJ::Object* log_panel_object, //log panel
		OBJ::Object* work_manager_object //work_manager
		) : Component(Kind::UIMANAGER, game_object)
	{
		
	}

	UIManager::~UIManager()
	{
		
	}

	void UIManager::setObject(
		OBJ::Object* UI_iron_kind_button_object, OBJ::Object* UI_wood_kind_button_object, //kind select button
		std::vector<OBJ::Object*> UI_bezier_focus_buttons_objects, //bezier focus point select button
		OBJ::Object* bezier_pannel_object, //bezier panel
		OBJ::Object* log_panel_object, //log panel
		OBJ::Object* work_manager_object
	)
	{
		this->UI_iron_kind_button_object = UI_iron_kind_button_object;
		this->UI_wood_kind_button_object = UI_wood_kind_button_object;
		this->UI_bezier_focus_buttons_objects = UI_bezier_focus_buttons_objects;
		this->bezier_pannel_object = bezier_pannel_object;
		this->log_panel_object = log_panel_object;
		this->work_manager_object = work_manager_object;

		setPieceKind(new std::string("wood"));
		setLength(new float(3.0f));
		setRadius(new float(0.5f));
		setFocus(new std::string("0"));
		control_points.clear();
		control_points.resize(4);
		index_map.clear();
		this->temp_constraint = NULL;

		((Button*)UI_wood_kind_button_object->getComponent(Kind::LABEL))->color = glm::vec3(0.5f);
	}

	void UIManager::start()
	{

	}

	void UIManager::update()
	{
		
	}

	void UIManager::setPieceKind(void* kind)//string
	{
		std::string kind_string(*((std::string*)kind));
		if (kind_string == "wood")
		{
			this->piece_kind = WorkPiece::PieceKind::WOOD;
		}
		else if (kind_string == "iron")
		{
			this->piece_kind = WorkPiece::PieceKind::IRON;
		}
		else
		{
			return;
		}

		Button* focus_button = NULL;
		Button* nofocus_button = NULL;
		if (piece_kind == WorkPiece::PieceKind::WOOD)
		{
			focus_button = (Button*)UI_wood_kind_button_object->getComponent(Kind::LABEL);
			nofocus_button = (Button*)UI_iron_kind_button_object->getComponent(Kind::LABEL);
		}
		else if (piece_kind == WorkPiece::PieceKind::IRON)
		{
			this->piece_kind = WorkPiece::PieceKind::IRON;
			focus_button = (Button*)UI_iron_kind_button_object->getComponent(Kind::LABEL);
			nofocus_button = (Button*)UI_wood_kind_button_object->getComponent(Kind::LABEL);
		}
		else
		{
			return;
		}
		focus_button->color = glm::vec3(0.5f);
		nofocus_button->color = glm::vec3(1.0f);

		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent("Piece kind: " + kind_string);
	}

	void UIManager::setLength(void* length)//float
	{
		float length_float = *(float*)(length);
		this->length = length_float;
		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent(std::string("Length: ") + std::to_string(length_float));
	}

	void UIManager::setRadius(void* radius)//float
	{
		float radius_float = *(float*)(radius);
		this->radius = radius_float;
		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent(std::string("Radius: ") + std::to_string(radius_float));
	}
	
	void UIManager::reset(void*)//NULL
	{
		WorkManager* work_manager = (WorkManager*)work_manager_object->getComponent(Kind::WORKMANAGER);
		work_manager->reset(piece_kind, length, radius);

		BezierPanel* bezier_panel = (BezierPanel*)bezier_pannel_object->getComponent(Kind::BEZIERPANEL);
		bezier_panel->reset();

		setFocus(new std::string("0"));
		index_map.clear();
		
		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent(std::string("Reset work piece"));
	}
	//bezier
	void UIManager::setFocus(void* index)//string
	{
		focus_index = std::stoi(*(std::string *)index);
		for (int i_button = 0; i_button < UI_bezier_focus_buttons_objects.size(); i_button++)
		{
			Button* button = (Button*)UI_bezier_focus_buttons_objects[i_button]->getComponent(Kind::LABEL);
			if (i_button == focus_index)
			{
				button->color = glm::vec3(0.5f);
			}
			else
			{
				button->color = glm::vec3(1.0f);
			}
		}

		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent(std::string("Focus: point ") + std::to_string(focus_index));
	}

	void UIManager::setBezierPoint(void* position)//glm::vec2
	{
		glm::vec2 position_vec2 = *(glm::vec2*)position;
		control_points[focus_index] = position_vec2;

		BezierPanel* bezier_panel = (BezierPanel*)bezier_pannel_object->getComponent(Kind::BEZIERPANEL);
		
		if (index_map.find(focus_index) != index_map.end())
		{
			bezier_panel->setTempPoints(index_map[focus_index], position_vec2);
		}
		else
		{
			index_map[focus_index] = bezier_panel->setTempPoints(-1, position_vec2);
		}

		//test valid
		bool valid_flag = true;
		for (int i = 0; i < 3; i++)
		{
			if (control_points[i].x > control_points[i + 1].x)
			{
				valid_flag = false;
			}
		}

		if (index_map.size() == 4 && valid_flag)
		{
			std::pair<float, float> x_interval;
			x_interval.first = control_points[0].x;
			x_interval.second = control_points[3].x;
			temp_constraint = new BezierConstraint(x_interval, control_points);
			bezier_panel->setTempConstraint(temp_constraint);
		}
		else
		{
			temp_constraint = NULL;
			bezier_panel->setTempConstraint(NULL);
		}

		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent(std::string("Point: ") + std::to_string(position_vec2.x) + std::string(",") + std::to_string(position_vec2.y));
	}

	void UIManager::setBezier(void*)//NULL
	{
		bool valid_flag = temp_constraint != NULL;
		for (int i = 0; i < 3; i++)
		{
			if (control_points[i].x > control_points[i + 1].x)
			{
				valid_flag = false;
			}
		}

		if (!valid_flag)
		{
			Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
			text->setContent(std::string("Bezier points invalid"));
			return;
		}

		setFocus(new std::string("0"));
		index_map.clear();
		
		BezierPanel* bezier_panel = (BezierPanel*)bezier_pannel_object->getComponent(Kind::BEZIERPANEL);
		bezier_panel->clearTempPoints();
		bezier_panel->setTempConstraint(NULL);

		WorkManager* work_manager = (WorkManager*)work_manager_object->getComponent(Kind::WORKMANAGER);
		work_manager->addConstraint(temp_constraint, false);

		Text* text = (Text*)log_panel_object->getComponent(CMP::Kind::TEXT);
		text->setContent(std::string("Set bezier success"));
	}

	namespace CALLBACK
	{
		UIManager* instance;
		void* setPieceKind(void* kind)
		{
			instance->setPieceKind(kind);
			return NULL;
		}

		void* setLength(void* length)
		{
			instance->setLength(length);
			return NULL;
		}
		void* setRadius(void* radius)
		{
			instance->setRadius(radius);
			return NULL;
		}
		void* reset(void*temp)
		{
			instance->reset(NULL);
			return NULL;
		}
		//bezier
		void* setFocus(void* index) 
		{
			instance->setFocus(index);
			return NULL;
		}
		void* setBezierPoint(void* position)
		{
			instance->setBezierPoint(position);
			return NULL;
		}
		void* setBezier(void*)
		{
			instance->setBezier(NULL);
			return NULL;
		}
	}
}