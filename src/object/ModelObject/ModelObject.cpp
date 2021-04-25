#include "ModelObject.h"
#include "../../component/Transform/Transform.h"
#include "../../component/AutoColor/AutoColor.h"

namespace OBJ
{
	ModelObject::ModelObject(unsigned int object_id, const std::string& path, MAT::ShaderProgram* shaderProgram, GLenum data_RW_type): Object(object_id, MODEL)
	{
		CMP::Transform* transform = new CMP::Transform(this);
		this->setComponent(transform);

		this->model = new MOD::Model(path, shaderProgram);
		this->shaderProgram = shaderProgram;
	}

	ModelObject::ModelObject(unsigned int object_id, Prefab* prefab): Object(object_id, MODEL)
	{
		this->model = ((ModelObject*)prefab->object)->model;
		this->shaderProgram = ((ModelObject*)prefab->object)->shaderProgram;
		this->isPrefab = true;
		this->prefab = prefab;
		prefab->used_objects->insert(this);
		for (auto i = prefab->components->begin(); i != prefab->components->end(); i++)
		{
			this->components->insert(std::make_pair(i->first, CMP::Component::createComponent(i->second)));
			this->components->at(i->first)->game_object = this;
		}
	}

	ModelObject::~ModelObject()
	{
		delete model;
	}

	void ModelObject::render()
	{
		if (!this->isActive)
		{
			return;
		}
		renderChilds();

		shaderProgram->use();

		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		glm::vec4 background = glm::vec4(0, 0, 0, 0);
		int send_flag = 0;
		int use_position_flag = 0;
		CMP::AutoColor* auto_color = (CMP::AutoColor*)this->getComponent(CMP::Kind::AUTOCOLOR);
		if (auto_color != NULL)
		{
			color = auto_color->color;
			background = auto_color->background;
			send_flag = auto_color->send_flag;
			use_position_flag = auto_color->use_position_flag;
		}

		shaderProgram->setUniformValue("color", glm::value_ptr(color), 4);
		if (send_flag == 1)
		{
			shaderProgram->setUniformValue("use_position_flag", &use_position_flag, 1);
			auto_color->send_flag = 0;
		}

		glm::mat4 model_mat = ((CMP::Transform*)this->components->at(CMP::Kind::TRANSFORM))->model_mat;
		shaderProgram->setUniformMatrixValue("model", glm::value_ptr(model_mat), 4);

		glDisable(GL_TEXTURE_2D);
		model->render();
		glEnable(GL_TEXTURE_2D);
	}
}