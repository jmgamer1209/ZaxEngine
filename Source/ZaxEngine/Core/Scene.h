#pragma once
#include <vector>
#include "GameObject.h"
using namespace std;

struct SceneLighting
{
	float ambientColor[3];
	float ambientIntensity;
};

class Scene
{
public:
	vector<GameObject*> list;
	SceneLighting lightingSettings;

	void AddGameObject(GameObject* go)
	{
		list.push_back(go);
	}
};

