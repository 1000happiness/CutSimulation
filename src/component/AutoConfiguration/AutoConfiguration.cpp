#include "AutoConfiguration.h"
#include <glad/glad.h>

namespace CMP
{
	AutoConfiguration::AutoConfiguration(OBJ::Object* game_object): Component(Kind::AUTOCOMFIGURATION, game_object)
	{

	}

	AutoConfiguration::~AutoConfiguration()
	{
	}

	void AutoConfiguration::start()
	{
		if (clear_flag & GL_DEPTH_BUFFER_BIT)
		{
			glEnable(GL_DEPTH_TEST);
		}
	}

	void AutoConfiguration::update()
	{
		glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
