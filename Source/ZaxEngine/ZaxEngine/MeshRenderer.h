#pragma once
#include "AssetModel.h"
#include "Material.h"
#include "glad/glad.h"
#include "Camera.h"

class MeshRenderer
{
public:
    AssetMesh* mesh;
    Material* mat;

    float position[3] = { 0, 0, 0 };
    float rotationAxis[3] = { 0,1,0 };
    float rotationAngle = 45.0f;
    float scale[3] = { 1,1,1 };

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
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        //// vertex tangent
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        //// vertex bitangent
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    }

    void Draw(Camera &camera)
    {
        auto shaderProgram = mat->shader;
        shaderProgram->Use();

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        model = glm::translate(model, glm::vec3(position[0], position[1], position[2]));
        // 注意，此旋转是基于模型本身的轴，所以其实当轴不是正xyz时，旋转会看起来很奇怪。这是正常的，后面会调整为欧拉角显示
        model = glm::rotate(model, rotationAngle, glm::vec3(rotationAxis[0], rotationAxis[1], rotationAxis[2]));
        model = glm::scale(model, glm::vec3(scale[0], scale[1], scale[2]));

        view = camera.GetLookAt();
        projection = camera.GetProjection();

        shaderProgram->SetUniform("model", model);
        shaderProgram->SetUniform("view", view);
        shaderProgram->SetUniform("projection", projection);

        mat->Draw();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

