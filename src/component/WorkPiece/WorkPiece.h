#ifndef CMP_WORKPIECE_H
#define CMP_WORKPIECE_H

#include <string>
#include <vector>

#include "../Component/Component.h"
#include "../../material/Texture/Texture.h"
#include "../../util/Constraint/Constraint.h"

namespace CMP
{
    class WorkPiece : public Component
    {
    public:
        const float SLICE_WIDTH = 0.01f;
        enum class PieceKind
        {
            WOOD,
            IRON
        };

        class PieceSlice
        {
        public:
            float x_index;
            float radius;
            bool processed;
            
            PieceSlice() {
                processed = false;
                x_index = 0;
                radius = 0;
            }
            PieceSlice(float x_index, float radius) {
                processed = false;
                this->x_index = x_index;
                this->radius = radius;
            }
        };

        PieceKind piece_kind;
        
        std::vector<MAT::Texture>* textures;
        float x_length;
        float radius;

        WorkPiece(OBJ::Object *game_object = NULL, PieceKind piece_kind=PieceKind::WOOD, float x_length = 3.0f, float radius = 0.5f, std::vector<MAT::Texture>* textures = NULL);
        ~WorkPiece();

        void reset(PieceKind piece_kind = PieceKind::WOOD, float x_length = 3.0f, float radius = 0.5f, std::vector<MAT::Texture>* textures = NULL);

        void start();
        void update();
        void render(MAT::ShaderProgram* shader);

        bool setRadius(float x_index, float new_radius, bool force);

    private:
        PieceSlice* piece_slices;
        float* vertices;//position normal 
        unsigned int vertex_size;
        float angle_delta;//degree
        int vertex_num;
        unsigned int* indices;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        void setMaterial(MAT::ShaderProgram* shader, bool processed);
        void init();
    };
} // namespace CMP

#endif