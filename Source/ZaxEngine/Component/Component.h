#pragma once
#include "Core/GameObject.h"

class GameObject;

class Component
{
public:
	GameObject* gameObject;
	virtual void OnGui() {};
};
