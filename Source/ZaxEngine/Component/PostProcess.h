#pragma once
#include "Component/Component.h"
#include "Renderer/FrameBuffer.h"
#include "Core/Mesh.h"
#include "Core/ShaderProgram.h"
#include "Core/Application.h"
#include "glad/gl.h"
#include "imgui/imgui.h"

class PostProcess: public Component
{
private:
	FrameBuffer* frameBuffer1;
	//FrameBuffer* frameBuffer2;
    ShaderProgram* screenShaderProgram;

public:
	PostProcess() 
	{
        frameBuffer1 = new FrameBuffer(Application::viewportWidth, Application::viewportHeight);
        //frameBuffer2 = new FrameBuffer(Application::viewportWidth, Application::viewportHeight);
        screenShaderProgram = new ShaderProgram(Application::contentPath + "Shaders/PostProcess/inverse.vs", Application::contentPath + "Shaders/PostProcess/inverse.fs");
	}

	FrameBuffer* Draw(FrameBuffer& source)
	{
        if (Application::isViewportSizeChanged) frameBuffer1->ChangeSize(Application::viewportWidth, Application::viewportHeight);

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

    void OnGui() override
    {
        if (ImGui::TreeNode("PostProcess"))
        {
            ImGui::Text("Enable: "); ImGui::SameLine();
            ImGui::Checkbox("##Enable:", &enabled);
            ImGui::TreePop();
        }
    }
};

