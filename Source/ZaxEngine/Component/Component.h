#pragma once
#include "Core/GameObject.h"
#include "Core/ZObject.h"

class GameObject;
class ZObject;

class Component: public ZObject
{
public:
	bool enabled = true;
	GameObject* gameObject;
	virtual void OnGui() {}
};
