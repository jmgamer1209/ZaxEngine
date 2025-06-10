#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::Transform
{
	void Create(MonoObject* obj);
	void RegisterInternalCall();
}