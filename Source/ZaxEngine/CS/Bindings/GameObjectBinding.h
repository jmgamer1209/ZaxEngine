#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::GameObject
{
	void Create(MonoObject* obj);
	void CreateWithName(MonoObject* obj, MonoString* name);

	void RegisterInternalCall();
}