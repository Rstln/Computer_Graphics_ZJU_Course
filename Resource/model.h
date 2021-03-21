#ifndef MODEL_H
#define MODEL_H
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include "shader.h"
#include "mesh.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include "../global_param.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);
unsigned int loadTexture(char const* path);

class Model{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    Model() {};
    Model(const char *path);
    void Draw(Shader shader);

private:
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
