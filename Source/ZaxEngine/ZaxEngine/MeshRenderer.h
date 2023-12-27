#pragma once
#include "AssetModel.h"
#include "Materials/Material.h"
#include "glad/gl.h"
#include "Camera.h"
#include "Utils.h"
#include "Light.h"
#include "imgui/imgui.h"

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
        auto shaderProgram = mat->shader;
        shaderProgram->Use();

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        auto transform = gameObject->GetComponent<Transform>();

        model = glm::translate(model, Utils::ArrayToVec3(transform->position));
        // 注意，此旋转是基于模型本身的轴，所以其实当轴不是正xyz时，旋转会看起来很奇怪。这是正常的，后面会调整为欧拉角显示

        model = glm::rotate(model, glm::radians(transform->rotation[1]), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform->rotation[0]), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform->rotation[2]), glm::vec3(0, 0, 1));

        model = glm::scale(model, Utils::ArrayToVec3(transform->scale));

        view = camera->GetViewMatrix();
        projection = camera->GetProjection();

        shaderProgram->SetUniform3f("cameraPos", camera->gameObject->GetComponent<Transform>()->position);
        shaderProgram->SetUniform("model", model);
        shaderProgram->SetUniform("view", view);
        shaderProgram->SetUniform("projection", projection);

        auto forward = light->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform("lightDirection", forward);
        shaderProgram->SetUniform3f("lightColor", light->color);
        shaderProgram->SetUniform("ambientIntensity", ambient);

        mat->Draw();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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

