#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::Mesh
{
	void Create(MonoObject* obj);
	void CreateWithPath(MonoObject* obj, MonoString* path);

	void RegisterInternalCall();
}