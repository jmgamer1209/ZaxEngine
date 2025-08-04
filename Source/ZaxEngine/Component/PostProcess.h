#pragma once
#include "Component/Component.h"
#include "Renderer/FrameBuffer.h"
#include "Core/Mesh.h"
#include "Core/ShaderProgram.h"
#include "glad/gl.h"
#include "imgui/imgui.h"

class PostProcess: public Component
{
private:
	FrameBuffer* frameBuffer1;
	//FrameBuffer* frameBuffer2;
    ShaderProgram* screenShaderProgram;

public:
    PostProcess();

    FrameBuffer* Draw(FrameBuffer& source);

    void OnGui() override;
};

