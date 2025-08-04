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
	void ChangeRenderSize(int width, int height);


private:
	Scene* scene;
	Camera* camera;
	Light* directionalLight;
	vector<Light*> pointLights;
	vector<Light*> spotLights;
	vector<MeshRenderer*> renderers;
	vector<vector<MeshRenderer*>> rendererGroups;
	ShaderProgram* screenShaderProgram;
	ShaderProgram* shadowShader;
	ShaderProgram* depthShader;
public:
	FrameBuffer* frameBuffer;
	int renderWidth;
	int renderHeight;

private:
	void DrawShadow();
	void DrawShadow(Light* light);
	void DrawRenderers();
	void DrawOpaque();
	void DrawTransparent();
	void DrawGroupRenderers(const vector<MeshRenderer*>& group, BlendMode surface);
	void DrawRendererWithLight(MeshRenderer* renderer, Light* light);
	void SetGlobalShaderVar(MeshRenderer* renderer, Light* light, ShaderProgram* shaderProgram);
	FrameBuffer* DrawPostProcess(PostProcess* postProcess);
	void DrawSkybox(Skybox* skybox);
	void DrawQuad(FrameBuffer* frameBuffer);
	void DrawDepth();
};