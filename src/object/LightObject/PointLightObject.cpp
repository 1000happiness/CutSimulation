#include "PointLightObject.h"
#include "../Cube/Cube.h"

namespace OBJ
{
	PointLightObject::PointLightObject(
		unsigned int object_id,
		CMP::Transform* transform,
		CMP::AmbientLight* ambient_light,
		CMP::DiffuseLight* diffuse_light,
		CMP::SpecularLight* specular_light,
		Attenuation attenuation,
		std::string uniform_buffer_name,
		unsigned int uniform_buffer_offset,
		bool update_in_loop_flag,
		MAT::ShaderProgram* self_shader
	) : Object(object_id, POINTLIGHT)
	{
		this->setComponent(transform);
		if (ambient_light != NULL)
		{
			this->setComponent(ambient_light);
		}
		if (diffuse_light != NULL)
		{
			this->setComponent(diffuse_light);
		}
		if (specular_light != NULL)
		{
			this->setComponent(specular_light);
		}
		this->attenuation = attenuation;

		this->uniform_buffer_name = uniform_buffer_name;
		this->uniform_buffer_offset = uniform_buffer_offset;
		glGenBuffers(1, &uniform_buffer_object);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_object);
		glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		GLuint binding_point = MAT::shader_controller->getBufferBindingIndex(uniform_buffer_name);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, uniform_buffer_object);

		this->update_in_loop_flag = update_in_loop_flag;
		this->self_shader = self_shader;

		if (self_shader != NULL) {
			cube = new Cube(object_id, self_shader, transform);
		}
		else {
			cube = NULL;
		}
	}

	PointLightObject::PointLightObject(unsigned int object_id, Prefab* prefab) : Object(object_id, POINTLIGHT)
	{
		//TODO
	}

	PointLightObject::~PointLightObject()
	{

	}

	void PointLightObject::start()
	{
		if (!update_in_loop_flag)
		{
			sendData2GPU();
		}

		if (self_shader != NULL)
		{
			cube->sendData2GPU();
		}

		Object::start();
	}

	void PointLightObject::render()
	{
		if (update_in_loop_flag)
		{
			sendData2GPU();
		}
		renderChilds();
	}

	void PointLightObject::sendData2GPU()
	{
		CMP::AmbientLight* ambient_light = (CMP::AmbientLight*)getComponent(CMP::Kind::AMBIENTLIGHT);
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		if (ambient_light != NULL)
		{
			ambient = ambient_light->getAmbient();
		}

		CMP::DiffuseLight* diffuse_light = (CMP::DiffuseLight*)getComponent(CMP::Kind::DIFFUSELIGHT);
		glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		if (diffuse_light != NULL)
		{
			diffuse = diffuse_light->getDiffuse();
		}

		CMP::SpecularLight* specular_light = (CMP::SpecularLight*)getComponent(CMP::Kind::SPECULARLIGHT);
		glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
		if (specular_light != NULL)
		{
			specular = specular_light->getSpecular();
		}

		if(this->uniform_buffer_name != "")
		{
			glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_object);
	
			glBufferSubData(GL_UNIFORM_BUFFER, uniform_buffer_offset + 1 * sizeof(glm::vec3), sizeof(glm::vec3), (float*)&attenuation);

			if (ambient_light != NULL)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, uniform_buffer_offset + 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(ambient));
			}

			if (diffuse_light != NULL)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, uniform_buffer_offset + 3 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(diffuse));
				CMP::Transform* transform = (CMP::Transform*)getComponent(CMP::Kind::TRANSFORM);
				glBufferSubData(GL_UNIFORM_BUFFER, uniform_buffer_offset + 0 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(transform->position));
			}

			if(specular_light != NULL)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, uniform_buffer_offset + 4 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(specular));
			}
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	
		if (self_shader != NULL)
		{
			glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			self_shader->use();
			self_shader->setUniformValue("light_color", glm::value_ptr(color), 4);
 			cube->render();
		}
	}
}

