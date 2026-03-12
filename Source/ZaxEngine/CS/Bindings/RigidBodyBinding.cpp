#include "RigidBodyBinding.h"
#include "mono/metadata/class.h"
#include "Component/RigidBody.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::RigidBody
{
	void Create(MonoObject* obj)
	{
		auto native = new ZaxEngine::Component::RigidBody();
		BindingHelper::SetNativePtr(obj, native);
		native->scriptObj = obj;
		Debug::Log("Create RigidBody Success");
	}

	void SetMotionType(MonoObject* obj, uint8_t motionType)
	{
		auto native = BindingHelper::GetNativeObj<ZaxEngine::Component::RigidBody*>(obj);
		native->SetMotionType(static_cast<JPH::EMotionType>(motionType));
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.RigidBody::Internal_Create", reinterpret_cast<void*>(Create));
		mono_add_internal_call("ZaxEngine.RigidBody::Internal_SetMotionType", reinterpret_cast<void*>(SetMotionType));
	}
};
