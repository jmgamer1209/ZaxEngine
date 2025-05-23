#pragma once
#include "Assets/AssetModel.h"
#include "Core/Material.h"
#include "glad/gl.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Component/Light.h"
#include "Core/Utils.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Mesh.h"

class MeshRenderer: public Component
{
public:
    Mesh* mesh;
    Material* mat;

	MeshRenderer(Mesh *mesh, Material* mat)
	{
        this->mesh = mesh;
        this->mat = mat;
        mesh->GenDrawData();
	}

    ~MeshRenderer()
    {

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

