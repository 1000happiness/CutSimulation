#ifndef CMP_UIMANAGER_H
#define CMP_UIMANAGER_H

#include <glm/vec3.hpp>
#include <vector>

#include "../WorkPiece/WorkPiece.h"
#include "../../component/Component/Component.h"
#include "../../util/Constraint/Constraint.h"

namespace CMP
{
	class UIManager : public Component
	{
	public:
		UIManager(
			OBJ::Object* game_object,//base object 
			OBJ::Object* UI_iron_kind_button_object, OBJ::Object* UI_wood_kind_button_object, //kind select button
			std::vector<OBJ::Object*> UI_bezier_focus_buttons_objects, //bezier focus point select button
			OBJ::Object* bezier_pannel_object, //bezier panel
			OBJ::Object* log_panel_object, //log panel
			OBJ::Object* work_manager_object //work_manager
			);
		~UIManager();

		void start();
		void update();
		void setObject(
			OBJ::Object* UI_iron_kind_button_object, OBJ::Object* UI_wood_kind_button_object, //kind select button
			std::vector<OBJ::Object*> UI_bezier_focus_buttons_objects, //bezier focus point select button
			OBJ::Object* bezier_pannel_object, //bezier panel
			OBJ::Object* log_panel_object, //log panel
			OBJ::Object* work_manager_object
		);
		
		//callback function
		//piece property
		void setPieceKind(void* kind);//string
		void setLength(void* length);//float
		void setRadius(void* radius);//float
		void reset(void*);//NULL
		//bezier
		void setFocus(void* index);//string
		void setBezierPoint(void* position);//glm::vec2
		void setBezier(void*);//NULL
		
	private:
		OBJ::Object* UI_iron_kind_button_object;
		OBJ::Object* UI_wood_kind_button_object; //kind select button
		std::vector<OBJ::Object*> UI_bezier_focus_buttons_objects;
		OBJ::Object* bezier_pannel_object;
		OBJ::Object* log_panel_object; //log panel
		OBJ::Object* work_manager_object;
		
		//status
		WorkPiece::PieceKind piece_kind;
		float length;
		float radius;

		int focus_index;
		std::vector<glm::vec2> control_points;
		std::map<int, int> index_map;
		Constraint* temp_constraint;
	};

	namespace CALLBACK
	{
		extern UIManager* instance;
		void* setPieceKind(void* kind);//string
		void *setLength(void* length);//float
		void *setRadius(void* radius);//float
		void *reset(void*);//NULL
		//bezier
		void *setFocus(void* index);//string
		void *setBezierPoint(void* position);//glm::vec2
		void *setBezier(void*);//NULL
	}
}

#endif // 
