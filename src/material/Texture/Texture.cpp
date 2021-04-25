#include "Texture.h"


namespace MAT
{   
    Texture::Texture(const std::string &tex_type, const std::string &img_path)
    {
        glGenTextures(1, &this->texture_id);
        this->tex_type = tex_type;
        bindTexture();
        setDefault();
        loadImage(img_path);
    }

    Texture::Texture()
    {
        
    }

    Texture::~Texture()
    {
    }

    void Texture::bindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, this->texture_id);
    }

    void Texture::loadImage(const std::string& img_path)
    {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, 0);
        //std::cout << "load texture from " << img_path << std::endl;
        
        if (data != NULL) {
            GLenum img_type = GL_RGB;
            if (nrChannels == 1)
                img_type = GL_RED;
            else if (nrChannels == 3)
                img_type = GL_RGB;
            else if (nrChannels == 4)
                img_type = GL_RGBA;
            //std::cout << nrChannels << std::endl;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, img_type, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void Texture::setTexParam(GLenum name, int param)
    {
        glTexParameteri(GL_TEXTURE_2D, name, param);
    }

    void Texture::setTexParam(GLenum name, float param)
    {
        glTexParameterf(GL_TEXTURE_2D, name, param);
    }

    void Texture::setTexParam(GLenum name, const int * param)
    {
        glTexParameteriv(GL_TEXTURE_2D, name, param);
    }
    
    void Texture::setTexParam(GLenum name, const float * param)
    {
        glTexParameterfv(GL_TEXTURE_2D, name, param);
    }
    
    void Texture::setDefault()
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
} // namespace MAT 