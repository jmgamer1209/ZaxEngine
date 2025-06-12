#pragma once
#include <mono/metadata/object.h>
#include "Core/GameObject.h"

namespace ZaxEngine::Binding::GameObject
{
	void Create(MonoObject* obj);
	void CreateWithName(MonoObject* obj, MonoString* name);
	void AddComponent(MonoObject* obj, ::GameObject* go, Component* comp);
	MonoObject* Internal_GetComponent(MonoObject* obj, MonoReflectionType* reflectionType);

	void RegisterInternalCall();
}