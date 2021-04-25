#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include "../Texture/Texture.h"

namespace MAT
{
    class ShaderProgram
    {
    private:
        unsigned int ID;
        std::map<GLenum, int> shaderMap;
        bool addStringShader(GLenum shaderType, const char * source);
    public:
        enum class LayoutType {
            POSTION_L=1,
            NORMAL_L,
            TEXTURE_L,
            TANGENT_L,
            BITANGENT_L,
            COLOR_L,
        };

        std::map<LayoutType, unsigned int> *layouts;
        std::map<std::string, std::string> *uniforms;
        std::map<std::string, std::vector<std::string>> *blocks;

        ShaderProgram();
        ShaderProgram(const std::string& configuration_path);
        ~ShaderProgram();

        int getID();
        bool link();
        void use();

        bool addShader(GLenum shaderType, const char * source);
        int getShader(GLenum);

        bool setUniformBlockBinding(const std::string & name, GLuint binding_point);
        bool setUniformValue(const char * name, bool *value, int number);
        bool setUniformValue(const char * name, int *value, int number);
        bool setUniformValue(const char * name, float *value, int number);
        bool setUniformMatrixValue(const char *name, float *value, int number);

        void linkTexture(const std::vector<Texture>& textures);
    };
} // namespace MAT

#endif