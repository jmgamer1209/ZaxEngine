#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::Skybox
{
	void Create(MonoObject* obj);
	void RegisterInternalCall();
}