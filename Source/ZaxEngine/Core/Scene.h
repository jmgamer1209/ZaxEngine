#pragma once
#include <vector>
#include "GameObject.h"
#include "Core/Color.h"
using namespace std;

struct SceneLighting
{
	Color ambientColor;
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

