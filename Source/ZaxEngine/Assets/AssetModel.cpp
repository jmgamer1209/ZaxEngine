#include "AssetModel.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Core/Debug.h"

AssetMesh::AssetMesh(vector<Vertex> vertices, vector<unsigned int> indices, unsigned int materialIndex)
{
    this->vertices = vertices;
    this->indices = indices;
    this->materialIndex = materialIndex;
}

void AssetModel::LoadModel(boost::filesystem::path path)
{
	// 测试导入 obj
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Debug::Log({ "错误::Assimp导入::", importer.GetErrorString() });
		return;
	}
	directory = path.parent_path();

    int meshNum = scene->mNumMeshes;
    int matNum = scene->mNumMaterials;

    for (int i = 0; i < matNum; i++)
    {
        auto mat = scene->mMaterials[i];
        materials.push_back(ProcessMaterial(mat, scene));
    }

    for (int i = 0; i < meshNum; i++)
    {
        auto mesh = scene->mMeshes[i];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
}

AssetMesh AssetModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Vector3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        
        // 法线
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
        }
        vertex.normal = vector;
        
        // uv 坐标
        vertex.texCoord = Vector2(0.0f, 0.0f);
        if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
        {
            // 暂时只取第1个的uv坐标
            Vector2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec;
        }

        // 切线和副切线
        vector = Vector3(0);
        if (mesh->HasTangentsAndBitangents())
        {            
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }

        vertices.push_back(vertex);
    }
    
    // 索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return AssetMesh(vertices, indices, mesh->mMaterialIndex);
}

AssetMaterial AssetModel::ProcessMaterial(aiMaterial* material, const aiScene* scene)
{
    AssetMaterial mat;
    mat.baseColor = LoadMaterialTexture(material, aiTextureType_DIFFUSE);
    mat.normal = LoadMaterialTexture(material, aiTextureType_HEIGHT);  // 对于 obj 模型的法线，需要使用 height 类型加载
    return mat;
}

AssetTexture AssetModel::LoadMaterialTexture(aiMaterial* mat, aiTextureType type)
{
     AssetTexture aTexuture;

     // 每个类型，只取一张
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString path;
        mat->GetTexture(type, i, &path);
        aTexuture.path = (directory / (path.C_Str())).string();
        break;
    }
    return aTexuture;
}