#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::Scene
{
	void CreateScene(MonoObject * obj);

	void RegisterInternalCall();
}