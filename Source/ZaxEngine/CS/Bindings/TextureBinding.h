#pragma once
#include <mono/metadata/object.h>
#include "Core/Texture.h"

namespace ZaxEngine::Binding::Texture
{
	MonoObject* Internal_Load(MonoString* path, TextureType type, bool flip_vertically);

	void RegisterInternalCall();
}