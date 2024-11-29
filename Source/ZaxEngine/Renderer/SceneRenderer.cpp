#include "SceneRenderer.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Core/Mesh.h"
#include "Core/Vector.h"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Init(int width, int height)
{
    frameBuffer = new FrameBuffer(width, height);
    screenShaderProgram = new ShaderProgram(Application::contentPath + "/Shaders/Common/screen.vs", Application::contentPath + "/Shaders/Common/screen.fs");
    shadowShader = new ShaderProgram(Application::contentPath + "/Shaders/Shadow/shadow.vs", Application::contentPath + "/Shaders/Shadow/shadow.fs");
    depthShader = new ShaderProgram(Application::contentPath + "/Shaders/Common/depth.vs", Application::contentPath + "/Shaders/Common/depth.fs");
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

        DrawOpaque();

        DrawSkybox(skybox);

        DrawTransparent();

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
        DrawShadow(pointLights[i]);
    }
}

void SceneRenderer::DrawShadow(Light* light)
{
    if (light->shadowFrameBuffer == nullptr)
    {
        if (light->type == LightType::Point)
        {
            light->shadowFrameBuffer = new ShadowCubeMapFrameBuffer(light->shadowMapSize, light->shadowMapSize);
        }
        else light->shadowFrameBuffer = new ShadowFrameBuffer(light->shadowMapSize, light->shadowMapSize);
    }
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, light->shadowFrameBuffer->GetID());
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, light->shadowFrameBuffer->GetBindTexture(), 0);  // 点光需要重新绑定，因为对于 point，后面代码会修改绑定


    glViewport(0, 0, light->shadowFrameBuffer->GetWidth(), light->shadowFrameBuffer->GetHeight());
    glClear(GL_DEPTH_BUFFER_BIT);

    if (light->type == LightType::Point) shadowShader->EnableKeyword("SHADOWCUBE");
    else shadowShader->DisableKeyword("SHADOWCUBE");

    for (size_t i = 0; i < renderers.size(); i++)
    {
        auto renderer = renderers[i];
        if (renderer->gameObject->isActive == false) continue;

        auto transform = renderer->gameObject->GetComponent<Transform>();

        shadowShader->Use();

        // 设置 MVP 矩阵
        glm::mat4 model(1.0f);

        model = glm::translate(model, transform->position.ToGLMVec());
        model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, transform->scale.ToGLMVec());
        
        auto projection = light->GetProjectionMatrix();
        shadowShader->SetUniform("model", model);
        shadowShader->SetUniform("projection", projection);

        if (light->type == LightType::Point)
        {
            vector<glm::mat4> viewList;
            auto transform = light->gameObject->GetComponent<Transform>();
            viewList.push_back(glm::lookAt(transform->position.ToGLMVec(), Vector3ToGLMVec(transform->position + transform->GetRight()), -transform->GetUp().ToGLMVec()));
            viewList.push_back(glm::lookAt(transform->position.ToGLMVec(), Vector3ToGLMVec(transform->position - transform->GetRight()), -transform->GetUp().ToGLMVec()));
            viewList.push_back(glm::lookAt(transform->position.ToGLMVec(), Vector3ToGLMVec(transform->position + transform->GetUp()), -transform->GetForward().ToGLMVec()));
            viewList.push_back(glm::lookAt(transform->position.ToGLMVec(), Vector3ToGLMVec(transform->position - transform->GetUp()), transform->GetForward().ToGLMVec()));
            viewList.push_back(glm::lookAt(transform->position.ToGLMVec(), Vector3ToGLMVec(transform->position - transform->GetForward()), -transform->GetUp().ToGLMVec()));
            viewList.push_back(glm::lookAt(transform->position.ToGLMVec(), Vector3ToGLMVec(transform->position + transform->GetForward()), -transform->GetUp().ToGLMVec()));

            for (size_t i = 0; i < 6; i++)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, (GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i), light->shadowFrameBuffer->GetBindTexture(), 0);
           //     auto view = light->GetViewMatrix();
                shadowShader->SetUniform("view", viewList[i]);
                shadowShader->SetUniform("light.range", light->range);
                shadowShader->SetUniform("light.position", transform->position);
                renderer->mesh->Draw();
            }
        }
        else
        {
            auto view = light->GetViewMatrix();
            shadowShader->SetUniform("view", view);
            renderer->mesh->Draw();
        }
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
    glCullFace(GL_BACK);

    // 对物体进行分组
    if (rendererGroups.size() == 0)
    {
        rendererGroups.push_back(vector<MeshRenderer*>());
        rendererGroups.push_back(vector<MeshRenderer*>());
    }
    else 
    {
        for (size_t i = 0; i < rendererGroups.size(); i++)
        {
            rendererGroups[i].clear();
        }
    }

    vector<MeshRenderer*>& transparentGroup = rendererGroups[1];
    auto camTransform = camera->gameObject->GetComponent<Transform>();

    for (size_t i = 0; i < renderers.size(); i++)
    {
        if (renderers[i]->mat->HasProperty("SurfaceType") == true &&  (SurfaceType)renderers[i]->mat->GetProperty("SurfaceType").intValue == SurfaceType::Transparent)
        {
            auto distance = Vector3::Dot(renderers[i]->gameObject->GetComponent<Transform>()->position - camTransform->position, camTransform->GetForward()); // 这个距离带正负号
            bool find = false;
            for (size_t j = 0; j < transparentGroup.size(); j++)
            {
                auto distanceItem = Vector3::Dot(transparentGroup[j]->gameObject->GetComponent<Transform>()->position - camTransform->position, camTransform->GetForward());
                if (distanceItem > distance)
                {
                    find = true;
                    transparentGroup.insert(transparentGroup.begin() + j, renderers[i]);
                    break;
                }
            }

            if (find == false)
            {
                transparentGroup.push_back(renderers[i]);
            }
        }
        else
        {
            rendererGroups[0].push_back(renderers[i]);
        }
    }
}

void SceneRenderer::DrawOpaque()
{
    auto index = (int)SurfaceType::Opaque;
    DrawGroupRenderers(rendererGroups[index], SurfaceType::Opaque);
}

void SceneRenderer::DrawTransparent()
{
    auto index = (int)SurfaceType::Transparent;
    DrawGroupRenderers(rendererGroups[index], SurfaceType::Transparent);
}

void SceneRenderer::DrawGroupRenderers(const vector<MeshRenderer*>& group, SurfaceType surface)
{
    glEnable(GL_DEPTH_TEST);

    for (size_t i = 0; i < group.size(); i++)
    {
        auto renderer = group[i];
        if (renderer->gameObject->isActive == false) continue;

        if (surface == SurfaceType::Opaque)
        {
            glDepthFunc(GL_LESS);
            glDisable(GL_BLEND);
        }
        else 
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthFunc(GL_LEQUAL);
        }

        DrawRendererWithLight(renderer, directionalLight);

        if (surface == SurfaceType::Opaque)
        {
            glDepthFunc(GL_LEQUAL);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
        }

        for (size_t i = 0; i < spotLights.size(); i++)
        {
            DrawRendererWithLight(renderer, spotLights[i]);
        }

        for (size_t i = 0; i < pointLights.size(); i++)
        {
            DrawRendererWithLight(renderer, pointLights[i]);
        }
    }
}

void SceneRenderer::DrawRendererWithLight(MeshRenderer* renderer, Light* light)
{
    ShaderProgram* shaderProgram = renderer->mat->shader;

    if (light->type == LightType::Point) shaderProgram->EnableKeyword("SHADOWCUBE");
    else shaderProgram->DisableKeyword("SHADOWCUBE");

    if (renderer->mat->HasProperty("NormalMap"))  shaderProgram->EnableKeyword("NORMALMAP");
    else shaderProgram->DisableKeyword("NORMALMAP");

    shaderProgram->Use();  

    SetGlobalShaderVar(renderer, light, shaderProgram);


    int texIndex = 0;
    // 设置材质
    renderer->mat->Draw(texIndex);

    shaderProgram->SetUniform("light.depthBias", light->shadowDepthBias);
    shaderProgram->SetUniform("light.type", (int)light->type);

    if (light->type == LightType::Directional)
    {
        // 设置平行光
        auto forward = light->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform("light.direction", forward);
        shaderProgram->SetUniform3f("light.color", light->color.FloatPTR());

         //设置 ShadowMap
        glActiveTexture(GL_TEXTURE0 + texIndex);
        glBindTexture(GL_TEXTURE_2D, light->shadowFrameBuffer->GetBindTexture());
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
        std::string varName = "light.";
        shaderProgram->SetUniform3f((varName + std::string("position")).c_str(), light->gameObject->GetComponent<Transform>()->position.FloatPTR());
        shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), light->color.FloatPTR());
        shaderProgram->SetUniform((varName + std::string("range")).c_str(), light->range);

        //设置 ShadowMap
        glActiveTexture(GL_TEXTURE0 + texIndex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, light->shadowFrameBuffer->GetBindTexture());
        shaderProgram->SetUniform("shadowCubeMap", texIndex);
        texIndex++;
        auto lightView = light->GetViewMatrix();
        auto lightProjection = light->GetProjectionMatrix();
        shaderProgram->SetUniform("lightView", lightView);
        shaderProgram->SetUniform("lightProjection", lightProjection);
    }
    else if (light ->type == LightType::Spot)
    {
        // 设置聚光
        string varName = "light.";
        shaderProgram->SetUniform3f((varName + std::string("position")).c_str(), light->gameObject->GetComponent<Transform>()->position.FloatPTR());
        auto forward = light->gameObject->GetComponent<Transform>()->GetForward();
        shaderProgram->SetUniform((varName + std::string("direction")).c_str(), forward);
        shaderProgram->SetUniform3f((varName + std::string("color")).c_str(), light->color.FloatPTR());
        shaderProgram->SetUniform((varName + std::string("range")).c_str(), light->range);
        shaderProgram->SetUniform((varName + std::string("cosInner")).c_str(), cosf(light->innerAngle * (float)M_PI / 180.0f));
        shaderProgram->SetUniform((varName + std::string("cosOuter")).c_str(), cosf(light->outerAngle * (float)M_PI / 180.0f));
        shaderProgram->SetUniform((varName + std::string("tanhalf")).c_str(), tanf(glm::radians(light->outerAngle / 2.0f)));
        shaderProgram->SetUniform((varName + std::string("near")).c_str(), 0.1f);
        shaderProgram->SetUniform((varName + std::string("far")).c_str(), light->range);

        //设置 ShadowMap
        glActiveTexture(GL_TEXTURE0 + texIndex);
        glBindTexture(GL_TEXTURE_2D, light->shadowFrameBuffer->GetBindTexture());
        shaderProgram->SetUniform("shadowMap", texIndex);
        texIndex++;
        auto lightView = light->GetViewMatrix();
        auto lightProjection = light->GetProjectionMatrix();
        shaderProgram->SetUniform("lightView", lightView);
        shaderProgram->SetUniform("lightProjection", lightProjection);
    }

    renderer->mesh->Draw();
}

void SceneRenderer::SetGlobalShaderVar(MeshRenderer* renderer, Light* light, ShaderProgram * shaderProgram)
{
    auto transform = renderer->gameObject->GetComponent<Transform>();

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
}

void SceneRenderer::DrawSkybox(Skybox* skybox)
{
    if (skybox == nullptr) return;
    if (skybox->gameObject->isActive == false) return;
    
    //skybox->SetCubeMap(pointLights[0]->shadowFrameBuffer->GetBindTexture()); // 用于测试 pointLight 阴影贴图
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

