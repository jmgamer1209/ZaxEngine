#pragma once
#include <vector>
#include "GameObject.h"
#include "Core/Color.h"
#include "filesystem/path.hpp"
#include "ZObject.h"

struct SceneLighting
{
	Color ambientColor;
	float ambientIntensity;
};

class Scene:public ZObject
{
public:
	vector<GameObject*> list;
	SceneLighting lightingSettings;

	Scene();

	void AddGameObject(GameObject* go);
	//void Serialize();
	void Unserialize(json::object obj) override;
};

