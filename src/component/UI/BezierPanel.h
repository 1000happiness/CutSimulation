#ifndef CMP_BEZIERPANEL_H
#define CMP_BEZIERPANEL_H

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/ext.hpp>

#include "../../material/Texture/Texture.h"
#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../Component/Component.h"
#include "../../util/Constraint/Constraint.h"


namespace CMP
{
	class BezierPanel : public Component
	{
	public:
		BezierPanel(OBJ::Object* game_object = NULL, glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(0.0f), float depth = 0.0f, OBJ::Object* work_manager_object = NULL, void* onclick(void*) = NULL);
		~BezierPanel();

		void start();
		void update();
		void render(MAT::ShaderProgram* shader);

		void setTempConstraint(Constraint *constraint);
		int setTempPoints(int index, glm::vec2 point);
		void clearTempPoints();
		void reset();

	protected:
		void testClick();
		bool click_state;

		std::vector<Constraint*> *upper_constraints;
		std::vector<Constraint*> *lower_constraints;
		Constraint* temp_constraint;
		std::vector<glm::vec2> temp_points; 
		glm::vec2 position;
		glm::vec2 scale;
		glm::vec2 standard_center;
		glm::vec2 standard_scale;
		glm::vec2 interval;
		float x_offset;
		float height;
		float depth;
		unsigned int background_VAO;
		unsigned int background_VBO;
		unsigned int cylinder_VAO;
		unsigned int cylinder_VBO;
		unsigned int constraints_VAO;
		unsigned int constraints_VBO;
		bool update_constraints_flag;
		bool first;
		void* (*onclick)(void*);
	};
}

#endif // !CMP_BEZIERPANEL_H

