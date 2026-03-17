#include "RigidBodyBinding.h"
#include "mono/metadata/class.h"
#include "Component/RigidBody.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::BoxCollider
{
	void Create(MonoObject* obj)
	{
		auto native = new ::BoxCollider();
		BindingHelper::SetNativePtr(obj, native);
		native->scriptObj = obj;
		Debug::Log("Create BoxCollider Success");
	}

	void SetShapeScale(MonoObject* obj, Vector3 scale)
	{
		auto native = BindingHelper::GetNativeObj<::BoxCollider>(obj);
		native.SetShapeScale(scale);
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.BoxCollider::Internal_Create", reinterpret_cast<void*>(Create));
		mono_add_internal_call("ZaxEngine.BoxCollider::Internal_SetShapeScale", reinterpret_cast<void*>(SetShapeScale));
	}
};
