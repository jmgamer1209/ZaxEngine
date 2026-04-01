#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Component/Skybox.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::Skybox {

	void Create(MonoObject* obj)
	{
		auto native = new ::Skybox();
		Binding::BindingHelper::SetNativePtr(obj, native);
		Debug::Log("Create Skybox Success");
	}

	void Internal_SetCubeMap(::Skybox* skybox, Texture* tex)
	{
		skybox->SetCubeMap(tex);
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Skybox::Internal_Create", reinterpret_cast<void*>(Create));
		mono_add_internal_call("ZaxEngine.Skybox::Internal_SetCubeMap", reinterpret_cast<void*>(Internal_SetCubeMap));
	}
}
