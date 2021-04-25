#include "SpecularLight.h"

namespace CMP
{
	SpecularLight::SpecularLight(OBJ::Object* game_object, float intensity, glm::vec3 color): Component(Kind::SPECULARLIGHT, game_object)
	{
		this->intensity = intensity;
		this->color = color;
		this->changeFlag = true;
	}

	SpecularLight::~SpecularLight()
	{
	}

	glm::vec3 SpecularLight::getSpecular()
	{
		return color * intensity;
	}

	void SpecularLight::start()
	{

	}

	void SpecularLight::update()
	{
	}
}