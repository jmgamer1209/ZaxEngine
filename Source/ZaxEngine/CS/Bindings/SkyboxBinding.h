#pragma once
#include <mono/metadata/object.h>
#include "Component/Skybox.h"
#include "Core/Texture.h"

namespace ZaxEngine::Binding::Skybox
{
	void Create(MonoObject* obj);
	void Internal_SetCubeMap(::Skybox* skybox, ::Texture* tex);
	void RegisterInternalCall();
}