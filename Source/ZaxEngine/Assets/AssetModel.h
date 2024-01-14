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

