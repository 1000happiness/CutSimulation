#ifndef MAT_SHADERCONTROLLER
#define MAT_SHADERCONTROLLER

#include <map>
#include <string>

#include <glad/glad.h>

#include "../ShaderProgram/ShaderProgram.h"

namespace MAT
{
	class ShaderController
	{
	public:
		ShaderController(std::string configuration_path);
		~ShaderController();

		GLuint getBufferBindingIndex(const std::string &name);
		ShaderProgram* getShader(const std::string& name);

	private:
		std::map<std::string, GLuint> binding_points_of_uniform_buffer;
		std::map<std::string, GLuint> binding_points_of_uniform_block;
		std::map<std::string, ShaderProgram *> shaders;
	};

	
}

#endif // !MAT_SHADERCONTROLLER
