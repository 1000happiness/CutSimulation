#include "ShaderController.h"
#include <json/json.h>

namespace MAT
{
	ShaderController::ShaderController(std::string configuration_path)
	{
        Json::Reader reader;
        Json::Value root;
        std::ifstream is;
        is.open(configuration_path);
        if (!reader.parse(is, root, false))
        {
            std::cerr << configuration_path << " json parse failed\n";
            return;
        }
        
        Json::Value shaders_json = root["shaders"];
        Json::Value::Members shaders_number = shaders_json.getMemberNames();
        for (unsigned int i = 0; i < shaders_number.size(); i++)
        {
            std::string shader_name = shaders_number[i];
            shaders[shader_name] = new ShaderProgram(shaders_json[shader_name].asString());
        }
        
        Json::Value uniform_buffer_json = root["uniform_buffer"];
         if (uniform_buffer_json.size() == 0)
        {
            return;
        }

        Json::Value::Members uniform_buffer_number = uniform_buffer_json.getMemberNames();
        for (unsigned int i = 0; i < uniform_buffer_number.size(); i++)
        {
            std::string uniform_buffer_name = uniform_buffer_number[i];
            binding_points_of_uniform_buffer[uniform_buffer_name] = uniform_buffer_json[uniform_buffer_name].asInt();
        }

        Json::Value uniform_block_json = root["uniform_blocks"];
        Json::Value::Members uniform_block_number = uniform_block_json.getMemberNames();
        for (unsigned int i = 0; i < uniform_block_number.size(); i++)
        {
            std::string uniform_block_name = uniform_block_number[i];
            binding_points_of_uniform_block[uniform_block_name] = uniform_block_json[uniform_block_name].asInt();
            for (auto shader = shaders.begin(); shader != shaders.end(); shader++)
            {
                shader->second->setUniformBlockBinding(uniform_block_name, binding_points_of_uniform_block[uniform_block_name]);
            }
        }
	}

	ShaderController::~ShaderController()
	{
	}

    GLuint ShaderController::getBufferBindingIndex(const std::string& name)
    {
        return binding_points_of_uniform_buffer[name];
    }

    ShaderProgram* ShaderController::getShader(const std::string& name)
    {
        return shaders[name];
    }
}