#include "Cube.h"
#include "../../component/Transform/Transform.h"

namespace OBJ
{
    const unsigned int Cube::VERTICE_NUMBER = 36;

    const float Cube::pos_refercence[] = 
    {
        -1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f, -1.0f, 
         1.0f,  1.0f, -1.0f, 
         1.0f,  1.0f, -1.0f, 
        -1.0f,  1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 

        -1.0f, -1.0f,  1.0f, 
         1.0f, -1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f, 
        -1.0f, -1.0f,  1.0f, 

        -1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f, -1.0f, 
        -1.0f, -1.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f, 

         1.0f,  1.0f,  1.0f, 
         1.0f,  1.0f, -1.0f, 
         1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f, 

        -1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f, -1.0f, 
         1.0f, -1.0f,  1.0f, 
         1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f,  1.0f, 
        -1.0f, -1.0f, -1.0f, 

        -1.0f,  1.0f, -1.0f, 
         1.0f,  1.0f, -1.0f, 
         1.0f,  1.0f,  1.0f, 
         1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f,  1.0f, 
        -1.0f,  1.0f, -1.0f, 
    };

    const float Cube::normal_reference[] =
    {
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,

         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,

        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,

         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,

         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,

         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f
    };

    const float Cube::tex_refercence[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    Cube::Cube(unsigned int object_id, MAT::ShaderProgram *shaderProgram, GLenum data_RW_type): Object(object_id, CUBE)
    {
        CMP::Transform * transform = new CMP::Transform(this);
        this->setComponent(transform);
        this->data_RW_type = data_RW_type;
        this->shaderProgram = shaderProgram;
        this->textures = new std::vector<MAT::Texture>();
        initVertices();
    }

    Cube::Cube(unsigned int object_id, MAT::ShaderProgram *shaderProgram, CMP::Transform *transform, GLenum data_RW_type): Object(object_id, CUBE)
    {
        this->shaderProgram = shaderProgram;
        transform->game_object = this;
        this->setComponent(transform);
        this->data_RW_type = data_RW_type;
        this->textures = new std::vector<MAT::Texture>();

        initVertices();
    }

    Cube::Cube(unsigned int object_id, Prefab *prefab): Object(object_id, CUBE)
    {
        this->VAO = ((Cube *)prefab->object)->VAO;
        this->VBO = ((Cube *)prefab->object)->VBO;
        this->vertices = ((Cube *)prefab->object)->vertices;
        this->shaderProgram = ((Cube *)prefab->object)->shaderProgram;
        this->data_RW_type = ((Cube*)prefab->object)->data_RW_type;
        this->textures = ((Cube*)prefab->object)->textures;
        this->isPrefab = true;
        this->prefab = prefab;
        prefab->used_objects->insert(this);
        for(auto i = prefab->components->begin(); i != prefab->components->end(); i++)
        {
            this->components->insert(std::make_pair(i->first, CMP::Component::createComponent(i->second)));
            this->components->at(i->first)->game_object = this;
        }
    }

    Cube::~Cube()
    {
        if(isPrefab && this->object_id == prefab->object->object_id)
        {
            return;
        }

        glDeleteVertexArrays(1, VAO);
        glDeleteBuffers(1, VBO);

        delete VAO;
        delete VBO;
        delete vertices;
    }

    void Cube::initVertices()
    {
        bool colorFlag = shaderProgram->layouts->find(MAT::ShaderProgram::LayoutType::COLOR_L) != shaderProgram->layouts->end();
        bool textureFlag = shaderProgram->layouts->find(MAT::ShaderProgram::LayoutType::TEXTURE_L) != shaderProgram->layouts->end();
        bool normalFlag = shaderProgram->layouts->find(MAT::ShaderProgram::LayoutType::NORMAL_L) != shaderProgram->layouts->end();

        unsigned int vertice_number = VERTICE_NUMBER;
        unsigned int vertice_size = POS_SIZE + (colorFlag ? COLOR_RGB_SIZE:0) + (textureFlag ? TEXTURE_SIZE:0);
        vertices = new float[vertice_number * vertice_size];
        for(unsigned int i = 0; i < vertice_number; i++)
        {
            unsigned int base_index = i * vertice_size;
            for(int k = 0; k < POS_SIZE; k++)
            {
                vertices[i * vertice_size + k] = pos_refercence[i * POS_SIZE + k];
            }

            if(colorFlag)
            {   
                vertices[i * vertice_size + POS_SIZE + 0] = 1.0f;
                vertices[i * vertice_size + POS_SIZE + 1] = 0.0f;
                vertices[i * vertice_size + POS_SIZE + 2] = 0.0f;
            }

            if(textureFlag)
            {   
                unsigned int color_size = (colorFlag ? COLOR_RGB_SIZE:0);
                for(unsigned int k = POS_SIZE + color_size; k < POS_SIZE + color_size + TEXTURE_SIZE; k++)
                {
                    vertices[i * vertice_size + k] = tex_refercence[i * TEXTURE_SIZE + k - POS_SIZE - color_size];
                }
            }
        }
        
        this->VAO = new unsigned int;
        this->VBO = new unsigned int;
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);
        // std::cout << "Vertice" << std::endl;
        
    }

    void Cube::sendData2GPU()
    {
        shaderProgram->use();
        bool colorFlag = shaderProgram->layouts->find(MAT::ShaderProgram::LayoutType::COLOR_L) != shaderProgram->layouts->end();
        bool textureFlag = shaderProgram->layouts->find(MAT::ShaderProgram::LayoutType::TEXTURE_L) != shaderProgram->layouts->end();
        unsigned int vertice_size = POS_SIZE + (colorFlag ? COLOR_RGB_SIZE:0) + (textureFlag ? TEXTURE_SIZE:0);

        glBindVertexArray(*VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, (VERTICE_NUMBER * vertice_size) * sizeof(float), vertices, data_RW_type);
        
        //position
        unsigned int pos_layout = shaderProgram->layouts->at(MAT::ShaderProgram::LayoutType::POSTION_L);
        unsigned int pos_dev = 0;
        glVertexAttribPointer(pos_layout, POS_SIZE, GL_FLOAT, GL_FALSE, vertice_size * sizeof(float), (void*)0);
        glEnableVertexAttribArray(pos_layout);

        //color
        if(colorFlag)
        {
            unsigned int color_layout = shaderProgram->layouts->at(MAT::ShaderProgram::LayoutType::COLOR_L);
            unsigned int color_dev = POS_SIZE;
            glVertexAttribPointer(color_layout, COLOR_RGB_SIZE, GL_FLOAT, GL_FALSE, vertice_size * sizeof(float), (void*)(color_dev * sizeof(float)));
            glEnableVertexAttribArray(color_layout);
        }
        
        //texture
        if(textureFlag)
        {
            unsigned int texture_layout = shaderProgram->layouts->at(MAT::ShaderProgram::LayoutType::TEXTURE_L);
            
            unsigned int texture_dev = POS_SIZE + (colorFlag ? COLOR_RGB_SIZE:0);
            // std::cout << texture_layout << texture_dev << std::endl;
            glVertexAttribPointer(texture_layout, TEXTURE_SIZE, GL_FLOAT, GL_FALSE, vertice_size * sizeof(float), (void*)(texture_dev * sizeof(float)));
            glEnableVertexAttribArray(texture_layout);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Cube::render()
    {
        if (!this->isActive)
        {
            return;
        }
        renderChilds();

        shaderProgram->use();
        if (textures != NULL) {
            shaderProgram->linkTexture(*textures);
        }

        glm::mat4 model_mat = ((CMP::Transform *)this->components->at(CMP::Kind::TRANSFORM))->model_mat;
        shaderProgram->setUniformMatrixValue("model", glm::value_ptr(model_mat), 4);
        glBindVertexArray(*VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);        
    }
} // namespace OBJ
