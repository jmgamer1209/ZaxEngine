#include "SceneRenderer.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "glm/gtc/matrix_transform.hpp"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Draw(Scene* scene)
{
    this->scene = scene;
    pointLights.clear();
    renderers.clear();

	for (size_t i = 0; i < scene->list.size(); i++)
	{
		auto tempCam = scene->list[i]->GetComponent<Camera>();
		if (tempCam != nullptr) camera = tempCam;

		auto tempLight = scene->list[i]->GetComponent<Light>();
        if (tempLight != nullptr)
        {
            if (tempLight->type == LightType::Directional) 
            {               
                this->directionalLight = tempLight;
            }
            else this->pointLights.push_back(tempLight);
        }

		auto tempRenderer = scene->list[i]->GetComponent<MeshRenderer>();
		if (tempRenderer != nullptr) renderers.push_back(tempRenderer);
	}

    DrawRenderers();
}

void SceneRenderer::DrawRenderers()
{
	for (size_t i = 0; i < renderers.size(); i++)
	{
		auto renderer = renderers[i];
        auto shaderProgram = renderer->mat->shader;
        auto transform = renderer->gameObject->GetComponent<Transform>();
        shaderProgram->Use();

        // ���� MVP ����
        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        model = glm::translate(model, Utils::ArrayToVec3(transform->position));
        // ע�⣬����ת�ǻ���ģ�ͱ������ᣬ������ʵ���᲻����xyzʱ����ת�ῴ��������֡����������ģ���������Ϊŷ������ʾ
        model = glm::rotate(model, glm::radians(transform->rotation[1]), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform->rotation[0]), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform->rotation[2]), glm::vec3(0, 0, 1));

        model = glm::scale(model, Utils::ArrayToVec3(transform->scale));

        view = camera->GetViewMatrix();
        projection = camera->GetProjection();
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

        
        shaderProgram->SetUniform3f("cameraPos", camera->gameObject->GetComponent<Transform>()->position);
        shaderProgram->SetUniform("model", model);
        shaderProgram->SetUniform("view", view);
        shaderProgram->SetUniform("projection", projection);
        shaderProgram->SetUniform("normalMatrix", normalMatrix);

        // ����ȫ�ֹ�
        shaderProgram->SetUniform("ambientIntensity", scene->lightingSettings.ambient);

        // ����ֱ���
        auto forward = directionalLight->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform("directionalLight.direction", forward);
        shaderProgram->SetUniform3f("directionalLight.color", directionalLight->color);
        
        // ���õ��
        shaderProgram->SetUniform("pointLightsNumber", (int)pointLights.size());
        for (size_t j = 0; j < pointLights.size(); j++)
        {
            std::string varName = "pointLights[" + std::to_string(j) + std::string("].");
            shaderProgram->SetUniform3f((varName+std::string("position")).c_str(), pointLights[i]->gameObject->GetComponent<Transform>()->position);
            shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), pointLights[i]->color);
            shaderProgram->SetUniform((varName + std::string("range")).c_str(), pointLights[i]->range);
        }

        // ���ò���
        renderer->mat->Draw();

        glBindVertexArray(renderer->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(renderer->mesh->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}
}