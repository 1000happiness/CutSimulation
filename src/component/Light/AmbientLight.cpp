#include "AmbientLight.h"

namespace CMP
{
	AmbientLight::AmbientLight(OBJ::Object* game_object, float intensity, glm::vec3 color): Component(Kind::AMBIENTLIGHT, game_object)
	{
		this->intensity = intensity;
		this->color = color;
		this->changeFlag = true;
	}

	AmbientLight::~AmbientLight()
	{
	}

	glm::vec3 AmbientLight::getAmbient()
	{
		return this->color * this->intensity;
	}

	void AmbientLight::start()
	{

	}

	void AmbientLight::update()
	{
	}
}