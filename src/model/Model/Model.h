#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../../material/Texture/Texture.h"
#include "../Mesh/Mesh.h"

namespace MOD
{
    class Model
    {
    public:
        // model data 
        std::map<std::string, MAT::Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh>    meshes;
        std::string directory;

        // constructor, expects a filepath to a 3D model.
        Model(std::string const& path, MAT::ShaderProgram* shader_program);

        // draws the model, and thus all its meshes
        void render();

    private:
        MAT::ShaderProgram* shader_program;
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes std::vector.
        void loadModel(std::string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a MAT::Texture struct.
        std::vector<MAT::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string tex_type);
    };
}


#endif 

