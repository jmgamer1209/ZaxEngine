#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Core/ShaderProgram.h"
#include <assimp/scene.h>
#include <unordered_map>
#include "Core/Vertex.h"
#include "Core/Mesh.h"
using namespace std;

struct AssetTexture {
    string path;
};

struct AssetMaterial
{
    AssetTexture baseColor;
    AssetTexture normal;
};

class AssetMesh
{
public:
    /*  网格数据  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int materialIndex;

    /*  函数  */
    AssetMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int materialIndex);

    Mesh* CreateMesh()
    {
        Mesh* mesh = new Mesh();
        mesh->vertices = vertices;
        mesh->indices = indices;
        return mesh;
    }
};

class AssetModel
{
public:
    /*  函数   */
    AssetModel(boost::filesystem::path path)
    {
        LoadModel(path);
    }
    /*  模型数据  */
    vector<AssetMesh> meshes;
    vector<AssetMaterial> materials;

    boost::filesystem::path directory;
    
    /*  函数   */
    void LoadModel(boost::filesystem::path path);
    //void ProcessNode(aiNode* node, const aiScene* scene);
    AssetMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    AssetMaterial ProcessMaterial(aiMaterial* material, const aiScene* scene);
    AssetTexture LoadMaterialTexture(aiMaterial* mat, aiTextureType type);

};

