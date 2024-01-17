#include "SceneRenderer.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Core/Mesh.h"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Init(int width, int height)
{
    frameBuffer = new FrameBuffer(width, height);
    screenShaderProgram = new ShaderProgram(Application::contentPath + "Shaders/Common/screen.vs", Application::contentPath + "Shaders/Common/screen.fs");
    shadowShader = new ShaderProgram(Application::contentPath + "Shaders/Shadow/shadow.vs", Application::contentPath + "Shaders/Shadow/shadow.fs");
    depthShader = new ShaderProgram(Application::contentPath + "Shaders/Common/depth.vs", Application::contentPath + "Shaders/Common/depth.fs");
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

        if (skybox == nullptr) skybox = scene->list[i]->GetComponent<Skybox>();

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

    if (viewMode == ViewMode::Depth) 
    {
        DrawDepth();
        DrawQuad(frameBuffer);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        DrawShadow();

        DrawRenderers();

        DrawSkybox(skybox);

        auto postBuffer = DrawPostProcess(camera->gameObject->GetComponent<PostProcess>());
        DrawQuad(postBuffer);
    }
}

void SceneRenderer::DrawDepth()
{
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetID());
    glViewport(0, 0, Application::viewportWidth, Application::viewportHeight);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < renderers.size(); i++)
    {
        auto renderer = renderers[i];
        if (renderer->gameObject->isActive == false) continue;

        auto transform = renderer->gameObject->GetComponent<Transform>();
        depthShader->Use();

        // 设置 MVP 矩阵
        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        model = glm::translate(model, Vector3ToGLMVec(transform->position));
        model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));

        model = glm::scale(model, Vector3ToGLMVec(transform->scale));

        view = camera->GetViewMatrix();
        projection = camera->GetProjection();
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

        depthShader->SetUniform("model", model);
        depthShader->SetUniform("view", view);
        depthShader->SetUniform("projection", projection);

        depthShader->Use();

        renderer->mesh->Draw();
    }
}

void SceneRenderer::DrawShadow()
{
    DrawShadow(directionalLight);
    for (size_t i = 0; i < spotLights.size(); i++)
    {
        DrawShadow(spotLights[i]);
    }

    for (size_t i = 0; i < pointLights.size(); i++)
    {
        //DrawShadow(pointLights[i]);
    }
}

void SceneRenderer::DrawShadow(Light* light)
{
    if (light->shadowFrameBuffer == nullptr)
    {
        light->shadowFrameBuffer = new ShadowFrameBuffer(light->shadowMapSize, light->shadowMapSize);
    }
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, light->shadowFrameBuffer->GetID());
    glViewport(0, 0, light->shadowFrameBuffer->GetWidth(), light->shadowFrameBuffer->GetHeight());
    glClear(GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < renderers.size(); i++)
    {
        auto renderer = renderers[i];
        if (renderer->gameObject->isActive == false) continue;

        auto transform = renderer->gameObject->GetComponent<Transform>();
        shadowShader->Use();

        // 设置 MVP 矩阵
        glm::mat4 model(1.0f);

        model = glm::translate(model, Vector3ToGLMVec(transform->position));
        model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, Vector3ToGLMVec(transform->scale));

        auto view = light->GetViewMatrix();
        auto projection = light->GetProjectionMatrix();

        shadowShader->SetUniform("model", model);
        shadowShader->SetUniform("view", view);
        shadowShader->SetUniform("projection", projection);

        renderer->mesh->Draw();
    }
}

void SceneRenderer::DrawRenderers()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetID());
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glViewport(0, 0, frameBuffer->GetWidth(), frameBuffer->GetHeight());
    glClearColor(0, 0, 0, 1);
    //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

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

        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
       
        DrawRendererWithLight(renderer,directionalLight);

        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        for (size_t i = 0; i < spotLights.size(); i++)
        {
            DrawRendererWithLight(renderer,spotLights[i]);
        }

        for (size_t i = 0; i < pointLights.size(); i++)
        {
            DrawRendererWithLight(renderer,pointLights[i]);
        }
	}

    glDisable(GL_BLEND);
}

void SceneRenderer::DrawRendererWithLight(MeshRenderer* renderer, Light* light)
{
    auto shaderProgram = renderer->mat->shader;
    int texIndex = 0;
    // 设置材质
    renderer->mat->Draw(texIndex);

    shaderProgram->SetUniform("light.depthBias", light->shadowDepthBias);

    if (light->type == LightType::Directional)
    {
        // 设置平行光
        shaderProgram->SetUniform("light.type", 0);
        auto forward = light->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform("light.direction", forward);
        shaderProgram->SetUniform3f("light.color", light->color.FloatPTR());


         //设置 ShadowMap
        glActiveTexture(GL_TEXTURE0 + texIndex);
        glBindTexture(GL_TEXTURE_2D, light->shadowFrameBuffer->GetDepthTexture());
        shaderProgram->SetUniform("shadowMap", texIndex);
        texIndex++;
        auto lightView = light->GetViewMatrix();
        auto lightProjection = light->GetProjectionMatrix();
        shaderProgram->SetUniform("lightView", lightView);
        shaderProgram->SetUniform("lightProjection", lightProjection);
    }
    else if (light->type == LightType::Point)
    {
        // 设置点光
        shaderProgram->SetUniform("light.type", 1);
        std::string varName = "light.";
        shaderProgram->SetUniform3f((varName + std::string("position")).c_str(), light->gameObject->GetComponent<Transform>()->position.FloatPTR());
        shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), light->color.FloatPTR());
        shaderProgram->SetUniform((varName + std::string("range")).c_str(), light->range);
    }
    else if (light ->type == LightType::Spot)
    {
        // 设置聚光
        shaderProgram->SetUniform("light.type", 2);
        string varName = "light.";
        shaderProgram->SetUniform3f((varName + std::string("position")).c_str(), light->gameObject->GetComponent<Transform>()->position.FloatPTR());
        auto forward = light->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform((varName + std::string("direction")).c_str(), forward);
        shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), light->color.FloatPTR());
        shaderProgram->SetUniform((varName + std::string("range")).c_str(), light->range);
        shaderProgram->SetUniform((varName + std::string("cosInner")).c_str(), cosf(light->innerAngle * (float)M_PI / 180.0f));
        shaderProgram->SetUniform((varName + std::string("cosOuter")).c_str(), cosf(light->outerAngle * (float)M_PI / 180.0f));
        shaderProgram->SetUniform((varName + std::string("near")).c_str(), 0.1f);
        shaderProgram->SetUniform((varName + std::string("far")).c_str(), light->range);

        //设置 ShadowMap
        glActiveTexture(GL_TEXTURE0 + texIndex);
        glBindTexture(GL_TEXTURE_2D, light->shadowFrameBuffer->GetDepthTexture());
        shaderProgram->SetUniform("shadowMap", texIndex);
        texIndex++;
        auto lightView = light->GetViewMatrix();
        auto lightProjection = light->GetProjectionMatrix();
        shaderProgram->SetUniform("lightView", lightView);
        shaderProgram->SetUniform("lightProjection", lightProjection);
    }

    renderer->mesh->Draw();
}

void SceneRenderer::DrawSkybox(Skybox* skybox)
{
    if (skybox == nullptr) return;
    if (skybox->gameObject->isActive == false) return;
    
    skybox->Draw(camera);
}

FrameBuffer* SceneRenderer::DrawPostProcess(PostProcess* postProcess)
{
    auto postFrameBuffer = postProcess->enabled == true? postProcess->Draw(*this->frameBuffer) : frameBuffer;
    return postFrameBuffer;
}

void SceneRenderer::DrawQuad(FrameBuffer *frameBuffer)
{
    // 将离屏图像绘制到屏幕
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Application::viewportWidth, Application::viewportHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenShaderProgram->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBuffer->GetTextureColorBuffer());
    screenShaderProgram->SetUniform("screenTex", 0);

    Mesh::GetQuadMesh()->Draw();
}

