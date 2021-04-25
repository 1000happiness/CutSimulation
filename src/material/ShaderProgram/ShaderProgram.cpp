#include "ShaderProgram.h"
#include <glad/glad.h>
#include <json/json.h>

namespace MAT
{
    ShaderProgram::ShaderProgram()
    {
        /*
        作用：生成着色器程序
        */
        ID = glCreateProgram();

        this->layouts = new std::map<MAT::ShaderProgram::LayoutType, unsigned int>();
        this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::POSTION_L, 0));
        this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::NORMAL_L, 1));
        this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::TEXTURE_L, 2));
        this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::TANGENT_L, 3));
        this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::BITANGENT_L, 4));
        this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::COLOR_L, 5));

        this->uniforms = new std::map<std::string, std::string>();
    }

    ShaderProgram::ShaderProgram(const std::string & configuration_path)
    {
        ID = glCreateProgram();

        Json::Reader reader;
        Json::Value root;
        std::ifstream is;
        is.open(configuration_path);
        if (!reader.parse(is, root, false))
        {
            std::cerr << configuration_path <<" json parse failed\n";
            return;
        }

        Json::Value sub_shaders_json = root["sub_shaders"];
        if (sub_shaders_json.isMember("vertex_shader"))
        {
            this->addShader(GL_VERTEX_SHADER, sub_shaders_json["vertex_shader"].asCString());
        }
        else
        {
            std::cerr << "vertex shader source not found\n";
            return;
        }
        if (sub_shaders_json.isMember("fragment_shader"))
        {
            this->addShader(GL_FRAGMENT_SHADER, sub_shaders_json["fragment_shader"].asCString());
        }
        else
        {
            std::cerr << "fragment shader source not found\n";
            return;
        }
        if (sub_shaders_json.isMember("geometry_shader"))
        {
            this->addShader(GL_GEOMETRY_SHADER, sub_shaders_json["geometry_shader"].asCString());
        }
        this->link();


        Json::Value layouts_json = root["layouts"];
        this->layouts = new std::map<MAT::ShaderProgram::LayoutType, unsigned int>();
        if (!layouts_json["position"].isNull()) {
            this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::POSTION_L, layouts_json["position"].asInt()));
        }
        if (!layouts_json["normal"].isNull()) {
            this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::NORMAL_L, layouts_json["normal"].asInt()));
        }
        if (!layouts_json["texture"].isNull()) {
            this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::TEXTURE_L, layouts_json["texture"].asInt()));
        }
        if (!layouts_json["tangent"].isNull()) {
            this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::TANGENT_L, layouts_json["tangent"].asInt()));
        }
        if (!layouts_json["bitangent"].isNull()) {
            this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::BITANGENT_L, layouts_json["bitangent"].asInt()));
        }
        if (!layouts_json["color"].isNull()) {
            this->layouts->insert(std::make_pair(MAT::ShaderProgram::LayoutType::COLOR_L, layouts_json["color"].asInt()));
        }

        Json::Value uniforms_json = root["uniforms"];
        Json::Value::Members uniforms_number = uniforms_json.getMemberNames();
        this->uniforms = new std::map<std::string, std::string>();
        for (unsigned int i = 0; i < uniforms_number.size(); i++)
        {
            std::string uniform_name = uniforms_number[i];
            uniforms->insert(std::make_pair(uniform_name, uniforms_json[uniform_name].asString()));
        }

        Json::Value blocks_json = root["blocks"];
        Json::Value::Members blocks_number = blocks_json.getMemberNames();
        this->blocks = new std::map<std::string, std::vector<std::string>>();
        for (unsigned int i = 0; i < blocks_number.size(); i++)
        {
            std::string block_name = blocks_number[i];
            blocks->insert(std::make_pair(block_name, std::vector<std::string>()));
            for (unsigned int k = 0; k < blocks_json[block_name].size(); k++)
            {
                blocks->at(block_name).push_back(blocks_json[block_name][k].asString());
            }
        }
    }

    ShaderProgram::~ShaderProgram()
    {
        delete layouts;
        delete uniforms;
    }

    int ShaderProgram::getID()
    {
        return ID;
    }

    bool ShaderProgram::link()
    {
        int success = 0;
        char infoLog[512];
        /*
        作用：链接着色器程序
        */
        glLinkProgram(ID);

        /*
        作用：获取是否链接成功
        参数：
            1.着色器程序ID@int
            2.状态值@GLenum
            3.是否成功链接（地址传递）@int *
        */
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            /*
            作用：获取详细日志信息
            参数：
                1.着色器程序ID@int
                2.日志字符串的大小@int
                3.待补充
                4.待填写的日志字符串@char *
            */
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
            return false;
        }

        for (auto shaderPair = shaderMap.begin(); shaderPair != shaderMap.end(); shaderPair++)
        {
            /*
            作用：删除着色器
            */
            glDeleteShader(shaderPair->second);
        }

        return true;
    }

    void ShaderProgram::use()
    {
        /*
        作用：使用着色器程序
        */
        glUseProgram(ID);
    }

    bool ShaderProgram::addShader(GLenum shaderType, const char *source)
    {
        std::string shaderCode;
        try
        {
            std::ifstream shaderFile;
            shaderFile.open(source);

            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();

            shaderFile.close();

            shaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        return addStringShader(shaderType, shaderCode.c_str());
    }

    int ShaderProgram::getShader(GLenum shaderType)
    {
        return shaderMap[shaderType];
    }

    bool ShaderProgram::setUniformBlockBinding(const std::string & name, GLuint binding_point)
    {
        if (blocks->find(name) == blocks->end())
        {
            return false;
        }

        int uniform_block_id = glGetUniformBlockIndex(ID, name.c_str());
        if (uniform_block_id == -1)
        {
            return false;
        }

        glUniformBlockBinding(ID, uniform_block_id, binding_point);
    }

    bool ShaderProgram::setUniformValue(const char *name, bool *value, int number)
    {
        if (number < 1 || number > 4)
        {
            return false;
        }
        /*
        作用：获取uniform数据的ID，后面可以根据这个ID更改相应的uniform的值
        参数：
            1.着色器程序@int
            2.uniform变量名
        补充：
            1.这个函数如果返回-1表示在程序中不存在这个变量
    */
        int uniformID = glGetUniformLocation(ID, name);
        if (uniformID == -1)
        {
            std::cout << name << " not found" << std::endl;
            return false;
        }

        switch (number)
        {
        case 1:
            glUniform1i(uniformID, (int)(value[0]));
            break;
        case 2:
            glUniform2i(uniformID, (int)(value[0]), (int)(value[1]));
            break;
        case 3:
            glUniform3i(uniformID, (int)(value[0]), (int)(value[1]), (int)(value[2]));
            break;
        case 4:
            /*
                作用：设定uniform的值
                参数：
                    1.ID
                    2.3.4.5.值
                补充：
                    1.不同类型的uniform用不同的函数进行更改
            */
            glUniform4i(uniformID, (int)(value[0]), (int)(value[1]), (int)(value[2]), (int)(value[3]));
            break;
        default:
            break;
        }
        return true;
    }

    bool ShaderProgram::setUniformValue(const char *name, int *value, int number)
    {
        if (number < 1 || number > 4)
        {
            return false;
        }

        int uniformID = glGetUniformLocation(ID, name);
        //std::cout << name << uniformID << std::endl;
        if (uniformID == -1)
        {
            std::cout << name << " not found" << std::endl;
            return false;
        }

        switch (number)
        {
        case 1:
            glUniform1i(uniformID, (value[0]));
            break;
        case 2:
            glUniform2i(uniformID, (value[0]), (value[1]));
            break;
        case 3:
            glUniform3i(uniformID, (value[0]), (value[1]), (value[2]));
            break;
        case 4:
            glUniform4i(uniformID, (value[0]), (value[1]), (value[2]), (value[3]));
            break;
        default:
            break;
        }
        return true;
    }

    bool ShaderProgram::setUniformValue(const char *name, float *value, int number)
    {
        if (number < 1 || number > 4)
        {
            return false;
        }

        int uniformID = glGetUniformLocation(ID, name);
        if (uniformID == -1)
        {
            std::cout << name << " not found" << std::endl;
            return false;
        }

        switch (number)
        {
        case 1:
            glUniform1f(uniformID, (value[0]));
            break;
        case 2:
            glUniform2f(uniformID, (value[0]), (value[1]));
            break;
        case 3:
            glUniform3f(uniformID, (value[0]), (value[1]), (value[2]));
            break;
        case 4:
            glUniform4f(uniformID, (value[0]), (value[1]), (value[2]), (value[3]));
            break;
        default:
            break;
        }
        return true;
    }

    bool ShaderProgram::setUniformMatrixValue(const char *name, float *value, int size)
    {
        if (size < 2 || size > 4)
        {
            return false;
        }

        int uniformID = glGetUniformLocation(ID, name);
        if (uniformID == -1)
        {
            std::cout << name << " not found" << std::endl;
            return false;
        }

        switch (size)
        {
            break;
        case 2:
            /*
            作用：传送uniform矩阵的值
            参数：
                1.uniformID@int
                2.传送矩阵的数量@int
                3.是否需要转置@bool
                4.矩阵值@float *
            */
            glUniformMatrix2fv(uniformID, 1, GL_FALSE, value);
            break;
        case 3:
            glUniformMatrix3fv(uniformID, 1, GL_FALSE, value);
            break;
        case 4:
            glUniformMatrix4fv(uniformID, 1, GL_FALSE, value);
            break;
        default:
            break;
        }
        return true;
    }

    void ShaderProgram::linkTexture(const std::vector<Texture>& textures)
    {
        // bind appropriate textures
        /*unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;*/
        for (int i = 0; i < textures.size() && i < 16; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            //string number;
            //string name = textures[i].type;
            //if (name == "texture_diffuse")
            //    number = std::to_string(diffuseNr++);
            //else if (name == "texture_specular")
            //    number = std::to_string(specularNr++); // transfer unsigned int to stream
            //else if (name == "texture_normal")
            //    number = std::to_string(normalNr++); // transfer unsigned int to stream
            //else if (name == "texture_height")
            //    number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            setUniformValue((std::string("texture") + std::to_string(i)).c_str(), &i, 1);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].texture_id);
        }
        /*glActiveTexture(GL_TEXTURE0);*/
    }

    bool ShaderProgram::addStringShader(GLenum shaderType, const char *source)
    {
        /*
        作用：根据类型生成着色器
        */
        int shader = glCreateShader(shaderType);
        /*
        作用：将源代码附着到着色器上
        参数：
            1.着色器ID@int
            2.着色器程序的数量@int
            3.着色器源程序@char **
            4.待补充
        */
        glShaderSource(shader, 1, &source, NULL);
        /*
        作用：编译着色器
        */
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return false;
        }
        glAttachShader(ID, shader);
        shaderMap.insert(std::make_pair(shaderType, shader));
        return true;
    }
} // namespace MAT
