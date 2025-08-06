#include "PostProcess.h"
#include "Core/Application.h"

PostProcess::PostProcess()
{
    frameBuffer1 = new FrameBuffer(Application::sceneRenderer->renderWidth, Application::sceneRenderer->renderHeight);
    //frameBuffer2 = new FrameBuffer(Application::viewportWidth, Application::viewportHeight);
    screenShaderProgram = new ShaderProgram(Application::contentPath / "/Shaders/PostProcess/inverse.vs", Application::contentPath / "/Shaders/PostProcess/inverse.fs");
}

FrameBuffer* PostProcess::Draw(FrameBuffer& source)
{
    frameBuffer1->ChangeSize(Application::sceneRenderer->renderWidth, Application::sceneRenderer->renderHeight);

    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1->GetID());
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenShaderProgram->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.GetTextureColorBuffer());
    screenShaderProgram->SetUniform("screenTex", 0);

    Mesh::GetQuadMesh()->Draw();

    return frameBuffer1;
}

void PostProcess::OnGui()
{
    if (ImGui::TreeNode("PostProcess"))
    {
        ImGui::Text("Enable: "); ImGui::SameLine();
        ImGui::Checkbox("##Enable:", &enabled);
        ImGui::TreePop();
    }
}
