#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// #include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>
#include <iostream>

#include "../../util/stb_image/stb_image.h"

namespace MAT
{
    class Texture
    {
    public:
        unsigned int texture_id;
        std::string tex_type;

        Texture(const std::string &name, const std::string &img_path);
        Texture();
        ~Texture();

        void bindTexture();
        void loadImage(const std::string& img_path);

        static void setTexParam(GLenum name, int param);
        static void setTexParam(GLenum name, float param);
        static void setTexParam(GLenum name, const int* param);
        static void setTexParam(GLenum name, const float* param);
        static void setDefault();
    };
    
} // namespace MAT

#endif