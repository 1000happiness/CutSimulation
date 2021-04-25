#ifndef OBJ_POINTLIGHTOBJECT_H
#define OBJ_POINTLIGHTOBJECT_H

#include "../Object/Object.h"
#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../../material/ShaderController/ShaderController.h"

#include "../../component/Light/AmbientLight.h"
#include "../../component/Light/DiffuseLight.h"
#include "../../component/Light/SpecularLight.h"
#include "../../component/Transform/Transform.h"

namespace MAT
{
	extern ShaderController* shader_controller;
}

namespace OBJ
{
	class PointLightObject : public Object
	{
	public:
		struct Attenuation
		{
			float constant;
			float linear;
			float quadratic;
			Attenuation() {
				this->constant = 1.0f;
				this->linear = 0.09f;
				this->quadratic = 0.032f;
			}
			Attenuation(float constant, float linear, float quadratic) {
				this->constant = constant;
				this->linear = linear;
				this->quadratic = quadratic;
			}
		};

		PointLightObject(
			unsigned int object_id,
			CMP::Transform* transform,
			CMP::AmbientLight* ambient_light,
			CMP::DiffuseLight* diffuse_light,
			CMP::SpecularLight* specular_light,
			Attenuation attenuation,
			std::string uniform_buffer_name,
			unsigned int uniform_buffer_offset = 0,
			bool update_in_loop_flag = true,
			MAT::ShaderProgram *self_shader = NULL
		);
		PointLightObject(unsigned int object_id, Prefab* prefab);
		~PointLightObject();

		void start();
		void render();
	private:
		Attenuation attenuation;

		std::string uniform_buffer_name;
		GLuint uniform_buffer_object;
		unsigned int uniform_buffer_offset;
		bool update_in_loop_flag;
		MAT::ShaderProgram* self_shader;
		Cube* cube;

		void sendData2GPU();
	};
}

#endif // !OBJ_POINTLIGHTOBJECT_H
