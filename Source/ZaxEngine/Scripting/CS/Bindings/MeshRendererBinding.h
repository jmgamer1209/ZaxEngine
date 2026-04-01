#pragma once
#include <mono/metadata/object.h>
#include "Core/Mesh.h"
#include "Core/Material.h"

namespace ZaxEngine::Binding::MeshRenderer
{
	void Internal_Create_WithMeshMat(MonoObject* obj, ::Mesh* mesh, ::Material* mat);

	void RegisterInternalCall();
}