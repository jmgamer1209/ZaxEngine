#include "SceneRenderer.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Core/Mesh.h"
#include "Core/QuadMesh.h"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Init(int width, int height)
{
    frameBuffer = new FrameBuffer(width, height);
    screenShaderProgram = new ShaderProgram(Application::contentPath + "Common/Shader/screen.vs", Application::contentPath + "Common/Shader/screen.fs");
}

void SceneRenderer::Draw(Scene* scene)
{
    this->scene = scene;
    pointLights.clear();
    spotLights.clear();
    renderers.clear();

    Skybox* skybox = nullptr;
	for (size_t i = 0; i < scene->list.size(); i++)
	{
		auto tempCam = scene->list[i]->GetComponent<Camera>();
		if (tempCam != nullptr) camera = tempCam;

        skybox = scene->list[i]->GetComponent<Skybox>();

		auto tempLight = scene->list[i]->GetComponent<Light>();
        if (tempLight != nullptr)
        {
            switch (tempLight->type)
            {
            case LightType::Directional:
                this->directionalLight = tempLight;
                break;
            case LightType::Point:
                this->pointLights.push_back(tempLight);
                break;
            case LightType::Spot:
                this->spotLights.push_back(tempLight);
                break;
            default:
                break;
            }
        }

		auto tempRenderer = scene->list[i]->GetComponent<MeshRenderer>();
		if (tempRenderer != nullptr) renderers.push_back(tempRenderer);
	}

    camera->OnViewportChange(Application::viewportWidth, Application::viewportHeight);
    // 如果窗口变化，需要重新设置纹理和rb的大小
    if (Application::isViewportSizeChanged)
    {
        frameBuffer->ChangeSize(Application::viewportWidth, Application::viewportHeight);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetID());
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawRenderers();
    DrawSkybox(skybox);
    DrawPostProcess(camera->gameObject->GetComponent<PostProcess>());
}

void SceneRenderer::DrawRenderers()
{
    glEnable(GL_DEPTH_TEST);

	for (size_t i = 0; i < renderers.size(); i++)
	{
		auto renderer = renderers[i];
        if (renderer->gameObject->isActive == false) continue;

        auto shaderProgram = renderer->mat->shader;
        auto transform = renderer->gameObject->GetComponent<Transform>();
        shaderProgram->Use();

        // 设置 MVP 矩阵
        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        model = glm::translate(model, Vector3ToGLMVec(transform->position));
        // 注意，此旋转是基于模型本身的轴，所以其实当轴不是正xyz时，旋转会看起来很奇怪。这是正常的，后面会调整为欧拉角显示
        model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));

        model = glm::scale(model, Vector3ToGLMVec(transform->scale));

        view = camera->GetViewMatrix();
        projection = camera->GetProjection();
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

        
        shaderProgram->SetUniform3f("cameraPos", camera->gameObject->GetComponent<Transform>()->position.FloatPTR());
        shaderProgram->SetUniform("model", model);
        shaderProgram->SetUniform("view", view);
        shaderProgram->SetUniform("projection", projection);
        shaderProgram->SetUniform("normalMatrix", normalMatrix);

        // 设置全局光
        shaderProgram->SetUniform3f("ambientColor", scene->lightingSettings.ambientColor.FloatPTR());
        shaderProgram->SetUniform("ambientIntensity", scene->lightingSettings.ambientIntensity);

        // 设置直射光
        auto forward = directionalLight->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform("directionalLight.direction", forward);
        shaderProgram->SetUniform3f("directionalLight.color", directionalLight->color.FloatPTR());
        
        // 设置点光
        shaderProgram->SetUniform("pointLightsNumber", (int)pointLights.size());
        for (size_t j = 0; j < pointLights.size(); j++)
        {
            auto light = pointLights[j];
            std::string varName = "pointLights[" + std::to_string(j) + std::string("].");
            shaderProgram->SetUniform3f((varName+std::string("position")).c_str(),  light->gameObject->GetComponent<Transform>()->position.FloatPTR());
            shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), light->color.FloatPTR());
            shaderProgram->SetUniform((varName + std::string("range")).c_str(), light->range);
        }

        // 设置聚光
        shaderProgram->SetUniform("spotLightsNumber", (int)spotLights.size());
        for (size_t j = 0; j < spotLights.size(); j++)
        {
            auto light = spotLights[j];
            std::string varName = "spotLights[" + std::to_string(j) + std::string("].");
            shaderProgram->SetUniform3f((varName + std::string("position")).c_str(), light->gameObject->GetComponent<Transform>()->position.FloatPTR());
            forward = light->gameObject->GetComponent<Transform>()->GetForward();
            shaderProgram->SetUniform((varName + std::string("direction")).c_str(), forward);
            shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), light->color.FloatPTR());
            shaderProgram->SetUniform((varName + std::string("range")).c_str(), light->range);
            shaderProgram->SetUniform((varName + std::string("cosInner")).c_str(), cosf(light->innerAngle * (float)M_PI / 180.0f));
            shaderProgram->SetUniform((varName + std::string("cosOuter")).c_str(), cosf(light->outerAngle * (float)M_PI / 180.0f));
        }

        // 设置材质
        renderer->mat->Draw();

        glBindVertexArray(renderer->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(renderer->mesh->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}
}

void SceneRenderer::DrawSkybox(Skybox* skybox)
{
    if (skybox == nullptr) return;
    if (skybox->gameObject->isActive == false) return;
    
    skybox->Draw(camera);
}

void SceneRenderer::DrawPostProcess(PostProcess* postProcess)
{
    auto postFrameBuffer = postProcess->enabled == true? postProcess->Draw(*this->frameBuffer) : frameBuffer;
    
    // 将离屏图像绘制到屏幕
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mesh::GetQuadMesh()->Setup();
    screenShaderProgram->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postFrameBuffer->GetTextureColorBuffer());
    screenShaderProgram->SetUniform("screenTex", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

