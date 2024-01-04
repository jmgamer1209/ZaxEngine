#pragma once
#include "Scene.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/MeshRenderer.h"

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();
	void Draw(Scene* scene);

private:
	Scene* scene;
	Camera* camera;
	Light* directionalLight;
	vector<Light*> pointLights;
	vector<Light*> spotLights;
	vector<MeshRenderer*> renderers;
	void DrawRenderers();
};