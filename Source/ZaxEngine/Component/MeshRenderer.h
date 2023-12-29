#pragma once
#include "Assets/AssetModel.h"
#include "Materials/Material.h"
#include "glad/gl.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/Light.h"
#include "Core/Utils.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

class MeshRenderer: public Component
{
public:
    AssetMesh* mesh;
    Material* mat;

	MeshRenderer(AssetModel *model, AssetMesh *mesh, Material* mat)
	{
        this->mesh = mesh;
        this->mat = mat;
        SetupMesh();
        mat->SetData(model, mesh);
	}

    ~MeshRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    unsigned int VBO, EBO, VAO;

    void SetupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // 切线和副切线暂时不用
        //// vertex tangent
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        //// vertex bitangent
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    }

    void Draw(Camera* camera, Light* light, float ambient)
    {

    }

    void OnGui() override
    {
        if (ImGui::TreeNode("MeshRenderer"))
        {
            mat->OnGui();

            ImGui::TreePop();
        }
        
    }
};

