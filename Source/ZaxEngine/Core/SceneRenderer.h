#pragma once
#include "Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"
#include "Core/FrameBuffer.h"
#include <Component/PostProcess.h>

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();
	void Init(int width, int height);
	void Draw(Scene* scene);
	void DrawPostProcess(PostProcess* postProcess);

public:
	FrameBuffer* frameBuffer;

private:
	Scene* scene;
	Camera* camera;
	Light* directionalLight;
	vector<Light*> pointLights;
	vector<Light*> spotLights;
	vector<MeshRenderer*> renderers;
	ShaderProgram* screenShaderProgram;

	void DrawRenderers();
};