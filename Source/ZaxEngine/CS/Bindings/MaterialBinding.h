#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::Material
{
	//void Create(MonoObject* obj);
	void Internal_Create_WithShader(MonoObject* obj, intptr_t shaderPtr);

	void RegisterInternalCall();
}