#pragma once
#include <vector>
#include "GameObject.h"
using namespace std;

struct SceneLighting
{
	float ambient;
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

