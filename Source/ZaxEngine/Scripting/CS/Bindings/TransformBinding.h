#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::Transform
{
	void Create(MonoObject* obj);
	::Vector3 Internal_GetPosition(MonoObject* obj);
	void Internal_SetPosition(MonoObject* obj, ::Vector3 position);
	void RegisterInternalCall();
}