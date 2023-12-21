#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <assimp/scene.h>
#include <unordered_map>

using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct AssetTexture {
    string path;
};

struct AssetMaterial
{
    AssetTexture baseColor;
};

class AssetMesh
{
public:
    /*  ��������  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int materialIndex;

    /*  ����  */
    AssetMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int materialIndex);
};

class AssetModel
{
public:
    /*  ����   */
    AssetModel(string path)
    {
        LoadModel(path);
    }
    /*  ģ������  */
    vector<AssetMesh> meshes;
    vector<AssetMaterial> materials;

    string directory;
    
    /*  ����   */
    void LoadModel(string path);
    //void ProcessNode(aiNode* node, const aiScene* scene);
    AssetMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    AssetMaterial ProcessMaterial(aiMaterial* material, const aiScene* scene);
    AssetTexture LoadMaterialTexture(aiMaterial* mat, aiTextureType type);

};

