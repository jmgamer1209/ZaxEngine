#include "MeshRendererBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "Component/MeshRenderer.h"

namespace ZaxEngine::Binding::MeshRenderer {

	void Internal_Create_WithMeshMat(MonoObject* obj, ::Mesh* mesh, ::Material* mat)
	{
		auto nativeObj = new ::MeshRenderer(mesh, mat);
		Binding::BindingHelper::SetNativePtr(obj, nativeObj);
			
		Debug::Log("Create MeshRenderer Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.MeshRenderer::Internal_Create_WithMeshMat", reinterpret_cast<void*>(Internal_Create_WithMeshMat));
	}
}
