#include "DiffuseLight.h"

namespace CMP
{
	DiffuseLight::DiffuseLight(OBJ::Object* game_object, glm::vec3 color): Component(Kind::DIFFUSELIGHT, game_object)
	{
		this->color = color;
	}

	DiffuseLight::~DiffuseLight()
	{
	}

	glm::vec3 DiffuseLight::getDiffuse()
	{
		return color;
	}

	void DiffuseLight::start()
	{

	}

	void DiffuseLight::update()
	{
	}
}