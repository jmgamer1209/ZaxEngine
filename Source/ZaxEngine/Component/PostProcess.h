#pragma once
#include "Component/Component.h"
#include "Core/FrameBuffer.h"
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
        screenShaderProgram = new ShaderProgram(Application::contentPath + "v0.11/screen.vs", Application::contentPath + "v0.11/screen.fs");
	}

	FrameBuffer* Draw(FrameBuffer& source)
	{
        // 将离屏图像绘制到四边形
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1->GetID());
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mesh::GetQuadMesh()->Setup();
        screenShaderProgram->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, source.GetTextureColorBuffer());
        screenShaderProgram->SetUniform("screenTex", 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

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

