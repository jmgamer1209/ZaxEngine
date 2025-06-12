#pragma once
#include "Core/GameObject.h"
#include "Core/ZObject.h"
#include "Scripting/ScriptObject.h"
using namespace ZaxEngine::Scripting;

class GameObject;
class ZObject;

class Component: public ZObject
{
public:
	bool enabled = true;
	GameObject* gameObject;
	ScriptObject scriptObj;
	virtual void OnGui() {}
};
