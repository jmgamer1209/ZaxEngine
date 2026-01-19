#include "RigidBodyBinding.h"
#include "mono/metadata/class.h"
#include "Component/RigidBody.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::RigidBody
{
	void Create(MonoObject* obj)
	{
		auto native = new ::RigidBody();
		BindingHelper::SetNativePtr(obj, native);
		native->scriptObj = obj;
		Debug::Log("Create RigidBody Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.RigidBody::Internal_Create", reinterpret_cast<void*>(Create));
	}
};
