#pragma once
#include "Core/Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/ShadowFrameBuffer.h"
#include "Component/PostProcess.h"
#include "Component/Skybox.h"

enum class ViewMode
{
	Lit, 
	Depth
};

class SceneRenderer
{
public:
	ViewMode viewMode = ViewMode::Lit;

public:
	SceneRenderer();
	~SceneRenderer();
	void Init(int width, int height);
	void Draw(Scene* scene);


private:
	Scene* scene;
	Camera* camera;
	Light* directionalLight;
	vector<Light*> pointLights;
	vector<Light*> spotLights;
	vector<MeshRenderer*> renderers;
	ShaderProgram* screenShaderProgram;
	ShaderProgram* shadowShader;
	ShaderProgram* depthShader;
	FrameBuffer* frameBuffer;

private:
	void DrawShadow();
	void DrawShadow(Light* light);
	void DrawRenderers();
	void DrawRendererWithLight(MeshRenderer* renderer, Light* light);
	FrameBuffer* DrawPostProcess(PostProcess* postProcess);
	void DrawSkybox(Skybox* skybox);
	void DrawQuad(FrameBuffer* frameBuffer);
	void DrawDepth();
};