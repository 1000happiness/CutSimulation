#include "WorkPiece.h"
#include "../Transform/Transform.h"
#include "../../object/Object/Object.h"
#include <random>
#include <glad/glad.h>

namespace CMP
{
    static std::vector<MAT::Texture>* wood_textures = new std::vector<MAT::Texture>();
    static std::vector<MAT::Texture>* iron_textures = new std::vector<MAT::Texture>();

    WorkPiece::WorkPiece(OBJ::Object* game_object, PieceKind piece_kind, float x_length, float radius, std::vector<MAT::Texture>* textures): Component(Kind::WORKPIECE, game_object)
    {
        if (wood_textures->empty()) {
            wood_textures->push_back(MAT::Texture("texture0", "./image/texture/wood_rough.jpg"));
            wood_textures->push_back(MAT::Texture("texture1", "./image/texture/wood_smooth2.jpg"));
        }
        if (iron_textures->empty()) {
            iron_textures->push_back(MAT::Texture("texture0", "./image/texture/iron_rough.jpg"));
            iron_textures->push_back(MAT::Texture("texture1", "./image/texture/iron_smooth.jpg"));
        }

        this->piece_kind = piece_kind;
        this->x_length = x_length;
        this->radius = radius;
        if (textures != NULL) {
            this->textures = textures;
        }
        else {
            switch (piece_kind)
            {
            case PieceKind::WOOD:
                this->textures = wood_textures;
                break;
            case PieceKind::IRON:
                this->textures = iron_textures;
                break;
            default:
                break;
            }
        }
       
        init();
    }
    
    WorkPiece::~WorkPiece()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete this->piece_slices;
        delete this->vertices;
        delete this->indices;
    }

    void WorkPiece::reset(PieceKind piece_kind, float x_length, float radius, std::vector<MAT::Texture>* textures)
    {
        this->piece_kind = piece_kind;
        this->x_length = x_length;
        this->radius = radius;
        if (textures != NULL) {
            this->textures = textures;
        }
        else {
            switch (piece_kind)
            {
            case PieceKind::WOOD:
                this->textures = wood_textures;
                break;
            case PieceKind::IRON:
                this->textures = iron_textures;
                break;
            default:
                break;
            }
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete this->piece_slices;
        delete this->vertices;
        delete this->indices;

        init();
    }

    void WorkPiece::start()
    {
    }

    static std::default_random_engine e;
    static std::uniform_real_distribution<float> u(-1.0f, 1.0f);

    void WorkPiece::update()
    {
        /*for (float x_index = 0.0f; x_index < x_length / 2; x_index += SLICE_WIDTH) {
            setRadius(x_index + 0.00001f, 0.3f);
        }*/
    }

    void WorkPiece::render(MAT::ShaderProgram* shader)
    {
        shader->use();

        //texture
        if (textures != NULL) {
            shader->linkTexture(*textures);
        }
        
        //uniform
        Transform* transform = (Transform*)game_object->getComponent(CMP::Kind::TRANSFORM);
        if (transform == NULL)
        {
            std::cout << "WorkPiece component depends on transform" << std::endl;
            return;
        }
        glm::mat4 model_mat = transform->model_mat;
        shader->setUniformMatrixValue("model", glm::value_ptr(model_mat), 4);
        
        bool surface_flag = false;
        shader->setUniformValue("surface_flag", &surface_flag, 1);

        shader->setUniformValue("x_length", &x_length, 1);
        
        //render side
        glBindVertexArray(VAO);
        unsigned int slice_num = unsigned int(x_length / SLICE_WIDTH);
        for (unsigned int i_slice = 0; i_slice < slice_num; i_slice++) {
            float x_index = piece_slices[i_slice].x_index;
            shader->setUniformValue("x_index", &x_index, 1);

            float left_radius = piece_slices[i_slice].radius;
            float right_radius = i_slice == slice_num - 1 ? piece_slices[i_slice].radius : piece_slices[i_slice + 1].radius;
            shader->setUniformValue("left_radius", &left_radius, 1);
            shader->setUniformValue("right_radius", &right_radius, 1);
            
            bool processed = piece_slices[i_slice].processed;
            shader->setUniformValue("processed", &processed, 1);
            setMaterial(shader, processed);
            glDrawElements(GL_TRIANGLES, vertex_num * 6, GL_UNSIGNED_INT, 0);
        }
        surface_flag = true;
        shader->setUniformValue("surface_flag", &surface_flag, 1);
        for (unsigned int i_slice = 0; i_slice < slice_num; i_slice++) {
            float x_index = piece_slices[i_slice].x_index;
            shader->setUniformValue("x_index", &x_index, 1);
            
            float left_radius = piece_slices[i_slice].radius;
            float right_radius = i_slice == slice_num - 1 ? piece_slices[i_slice].radius : piece_slices[i_slice + 1].radius;
            shader->setUniformValue("left_radius", &left_radius, 1);
            shader->setUniformValue("right_radius", &right_radius, 1);

            setMaterial(shader, true);
            glDrawElements(GL_TRIANGLES, vertex_num * 6, GL_UNSIGNED_INT, 0);
        }
        
        glBindVertexArray(0);
    }

    bool WorkPiece::setRadius(float x_index, float new_radius, bool force)
    {
        int index = int(x_index / SLICE_WIDTH);
        if (force)
        {
            this->piece_slices[index].radius = new_radius;
            this->piece_slices[index].processed = true;
            return true;
        }
        
        if (this->piece_slices[index].radius < new_radius)
        {
            return false;
        }
        
        if (index < int(x_length / SLICE_WIDTH) - 1 && this->piece_slices[index + 1].radius - this->piece_slices[index].radius > 0.1f)
        {
            return false;
        }

        if (this->piece_slices[index].radius - new_radius > 0.5f )
        {
            return false;
        }

        this->piece_slices[index].radius = new_radius;
        this->piece_slices[index].processed = true;
        return true;
    }

    void WorkPiece::setMaterial(MAT::ShaderProgram *shader, bool processed)
    {
        glm::vec3 ambient = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
        float shininess = 2.0f;
        if (piece_kind == PieceKind::WOOD && processed) {
            diffuse = glm::vec3(0.7f);
            specular = glm::vec3(0.5f);
            shininess = 8.0f;
        }
        if (piece_kind == PieceKind::WOOD && !processed) {
            diffuse = glm::vec3(0.5f);
            specular = glm::vec3(0.3f);
            shininess = 2.0f;
        }
        if (piece_kind == PieceKind::IRON && processed) {
            diffuse = glm::vec3(1.0f);
            specular = glm::vec3(1.0f);
            shininess = 2.0f;
        }
        if (piece_kind == PieceKind::IRON && !processed) {
            diffuse = glm::vec3(1.0f);
            specular = glm::vec3(1.0f);
            shininess = 16.0f;
        }
        shader->setUniformValue("material.ambient", glm::value_ptr(ambient), 3);
        shader->setUniformValue("material.diffuse", glm::value_ptr(diffuse), 3);
        shader->setUniformValue("material.specular", glm::value_ptr(specular), 3);
        shader->setUniformValue("material.shininess", &shininess, 1);
        return;
    }

    void WorkPiece::init()
    {
        //init pieces
        int slice_num = int(x_length / SLICE_WIDTH);
        this->piece_slices = new PieceSlice[slice_num];
        float x_index = 0;
        for (int i = 0; i < slice_num; i++) {
            this->piece_slices[i].processed = false;
            this->piece_slices[i].x_index = x_index;
            this->piece_slices[i].radius = radius;
            x_index += SLICE_WIDTH;
        }

        //init vertices 
        vertex_size = 8;
        angle_delta = 1.0f;
        vertex_num = int(360.0f / angle_delta);
        vertices = new float[vertex_size * 2 * vertex_num];

        float angle = 0;
        for (int i_vertex = 0; i_vertex < vertex_num; i_vertex++) {
            float radians_angle = glm::radians(angle);
            float y = glm::cos(radians_angle);
            float z = glm::sin(radians_angle);

            //point 1
            vertices[i_vertex * 2 * vertex_size + 0] = 0;
            vertices[i_vertex * 2 * vertex_size + 1] = y;
            vertices[i_vertex * 2 * vertex_size + 2] = z;
            vertices[i_vertex * 2 * vertex_size + 3] = 0;
            vertices[i_vertex * 2 * vertex_size + 4] = y;
            vertices[i_vertex * 2 * vertex_size + 5] = z;
            vertices[i_vertex * 2 * vertex_size + 6] = 0;// 0,0 -> 0,1 -> 0,0
            vertices[i_vertex * 2 * vertex_size + 7] = angle / 360.0f;

            //point 2
            vertices[i_vertex * 2 * vertex_size + 8] = SLICE_WIDTH;
            vertices[i_vertex * 2 * vertex_size + 9] = y;
            vertices[i_vertex * 2 * vertex_size + 10] = z;
            vertices[i_vertex * 2 * vertex_size + 11] = 0;
            vertices[i_vertex * 2 * vertex_size + 12] = y;
            vertices[i_vertex * 2 * vertex_size + 13] = z;
            vertices[i_vertex * 2 * vertex_size + 14] = 1;// 1,0 -> 1,1 -> 1,0
            vertices[i_vertex * 2 * vertex_size + 15] = angle / 360.0f;

            angle += angle_delta;
        }

        //init indices
        unsigned int indices_num = vertex_num * 2;
        indices = new unsigned int[indices_num * 3];
        for (int i_side = 0; i_side < vertex_num; i_side++) {
            indices[i_side * 2 * 3 + 0] = i_side * 2 + 0;
            indices[i_side * 2 * 3 + 1] = i_side * 2 + 1;
            indices[i_side * 2 * 3 + 2] = i_side * 2 + 2;
            indices[i_side * 2 * 3 + 3] = i_side * 2 + 1;
            indices[i_side * 2 * 3 + 4] = i_side * 2 + 2;
            indices[i_side * 2 * 3 + 5] = i_side * 2 + 3;
        }
        indices[vertex_num * 2 * 3 - 6] = vertex_num * 2 - 2;//718 -> 718
        indices[vertex_num * 2 * 3 - 5] = vertex_num * 2 - 1;//719 -> 719
        indices[vertex_num * 2 * 3 - 4] = 0;                 //720 -> 0
        indices[vertex_num * 2 * 3 - 3] = vertex_num * 2 - 1;//719 -> 719
        indices[vertex_num * 2 * 3 - 2] = 0;                 //720 -> 0
        indices[vertex_num * 2 * 3 - 1] = 1;                 //721 -> 1

        //init gl object
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_size * 2 * vertex_num * sizeof(float), this->vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_num * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        int position_layout = 0;
        glVertexAttribPointer(position_layout, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)(0 * sizeof(float)));
        glEnableVertexAttribArray(position_layout);
        int normal_layout = 1;
        glVertexAttribPointer(normal_layout, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(normal_layout);
        int texture_layout = 2;
        glVertexAttribPointer(texture_layout, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(texture_layout);
        glBindVertexArray(0);
    }
} // namespace CMP
