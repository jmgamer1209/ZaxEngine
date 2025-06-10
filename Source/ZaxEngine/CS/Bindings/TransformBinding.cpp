#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Component/Transform.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::Transform {

	void Create(MonoObject* obj)
	{
		auto native = new ::Transform();
		Binding::BindingHelper::SetNativePtr(obj, native);
		Debug::Log("Create Transform Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Transform::Internal_Create", reinterpret_cast<void*>(Create));
	}
}
