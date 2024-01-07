#pragma once
#include "Core/GameObject.h"

class GameObject;

class Component
{
public:
	bool enabled = true;
	GameObject* gameObject;
	virtual void OnGui() {}
};
