#pragma once
#include <vector>
#include "GameObject.h"
#include "Core/Color.h"
#include "filesystem/path.hpp"

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

	void AddGameObject(GameObject* go);
	void Serialize();
	void DeSerialize(boost::filesystem::path path);
};

