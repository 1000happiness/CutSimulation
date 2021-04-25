#ifndef CMP_WORKMANAGER_H
#define CMP_WORKMANAGER_H

#include <glm/vec3.hpp>
#include <vector>

#include "../WorkPiece/WorkPiece.h"
#include "../../component/Component/Component.h"
#include "../../util/Constraint/Constraint.h"

namespace CMP
{
	class WorkManager : public Component
	{
	public:
		std::vector<Constraint*> lower_constraints;
		std::vector<Constraint*> upper_constraints;

		WorkManager(OBJ::Object* game_object = NULL, OBJ::Object *work_piece_object = NULL, OBJ::Object *cutter_object = NULL, OBJ::Object *camera_object = NULL, OBJ::Object *particle_system_object = NULL);
		~WorkManager();

		void start();
		void update();
		void reset(WorkPiece::PieceKind piece_kind, float length, float radius);
		void addConstraint(Constraint*, bool upper_flag);
	private:
		enum class Mode
		{
			RUN,
			CONFIGURE
		};

		enum class CutState
		{
			EMPTY,
			CUT,
			STOP
		};

		const glm::vec3 wood_particle_color = glm::vec3(160.0f / 255.0f, 115.0f / 255.0f, 64.0f / 255.0f);
		const glm::vec3 iron_particle_color = glm::vec3(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f);

		OBJ::Object* work_piece_object;
		OBJ::Object* cutter_object;
		OBJ::Object* camera_object;
		OBJ::Object* particle_system_object;
		Mode mode;

		void init();
		void switchMode();
		void moveCutter();
		CutState cut(glm::vec3 cutter_position);
	};
}

#endif // 
