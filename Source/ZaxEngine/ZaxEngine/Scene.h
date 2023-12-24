#pragma once
#include <vector>
#include "GameObject.h"
using namespace std;

class Scene
{
public:
	vector<GameObject*> list;
	void AddGameObject(GameObject* go)
	{
		list.push_back(go);
	}
};

