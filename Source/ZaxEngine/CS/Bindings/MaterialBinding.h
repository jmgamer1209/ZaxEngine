#pragma once
#include <mono/metadata/object.h>
#include "Core/Material.h"

namespace ZaxEngine::Binding::Material
{
	//void Create(MonoObject* obj);
	void Internal_Create_WithShader(MonoObject* obj, intptr_t shaderPtr);

	void Internal_SetProperty_Texture(::Material* mat, MonoString* name, ::Texture* value);
	void Internal_SetProperty_Float(::Material* mat, MonoString* name, float value);
	void Internal_SetProperty_Int(::Material* mat, MonoString* name, int32_t value);

	void RegisterInternalCall();
}