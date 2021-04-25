#include "AutoRotation.h"
#include "../Transform/Transform.h"
#include "../../object/Object/Object.h"
#include <iostream>

namespace CMP
{
	AutoRotation::AutoRotation(OBJ::Object* game_object, float angle, glm::vec3 axis): Component(Kind::AUTOROTATION, game_object)
	{
		this->angle = angle;
		this->axis = axis;
		this->rotation_flag = true;
	}

	AutoRotation::~AutoRotation()
	{
	}

	void AutoRotation::start()
	{

	}

	void AutoRotation::update()
	{
		if (!rotation_flag)
		{
			return;
		}

		Transform* transform = (Transform*)game_object->getComponent(CMP::Kind::TRANSFORM);
		if (transform != NULL)
		{
			glm::mat4 rotation_mat = transform->rotation_mat;
			rotation_mat = glm::rotate(rotation_mat, glm::radians(angle), axis);
			transform->set(Transform::ROTATION_MAT, rotation_mat);
		}
	}

	void AutoRotation::set(PropertyType property_type, glm::vec3 value)
	{
		changeFlag = true;
		switch (property_type)
		{
		case AXIS:
			this->axis = value;
		default:
			changeFlag = false;
			break;
		}
	}

	void AutoRotation::set(PropertyType property_type, float value)
	{
		changeFlag = true;
		switch (property_type)
		{
		case ANGLE:
			this->angle = value;
		default:
			changeFlag = false;
			break;
		}
	}

	void AutoRotation::set(PropertyType property_type, bool value)
	{
		changeFlag = false;
		switch (property_type)
		{
		case ROTATION_FLAG:
			this->rotation_flag = value;
		default:
			changeFlag = false;
			break;
		}
	}
}