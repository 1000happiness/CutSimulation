#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include <string>

#include "../../model/Model/Model.h"

#include "../Object/Object.h"

namespace OBJ
{
	class ModelObject: public Object
	{
	public:
		MAT::ShaderProgram* shaderProgram;

		ModelObject(unsigned int object_id, const std::string &path, MAT::ShaderProgram* shaderProgram, GLenum data_RW_type=GL_STATIC_DRAW);
		ModelObject(unsigned int object_id, Prefab* prefab);
		~ModelObject();

		void render();
	private:
		MOD::Model *model;
	};
}

#endif // !MODELOBJECT_H

