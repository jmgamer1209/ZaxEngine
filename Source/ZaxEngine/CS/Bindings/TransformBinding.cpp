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
		native->scriptObj = obj;
		Debug::Log("Create Transform Success");
	}

	::Vector3 Internal_GetPosition(MonoObject* obj)
	{
		auto transform = Binding::BindingHelper::GetNativeObj<::Transform*>(obj);
		return transform->position;
	}

	void Internal_SetPosition(MonoObject* obj, ::Vector3 value) 
	{
		auto transform = Binding::BindingHelper::GetNativeObj<::Transform*>(obj);
		transform->position = value;
	}

	::Vector3 Internal_GetRotation(MonoObject* obj)
	{
		auto transform = Binding::BindingHelper::GetNativeObj<::Transform*>(obj);
		return transform->rotation;
	}

	void Internal_SetRotation(MonoObject* obj, ::Vector3 value)
	{
		auto transform = Binding::BindingHelper::GetNativeObj<::Transform*>(obj);
		transform->rotation = value;
	}

	::Vector3 Internal_GetScale(MonoObject* obj)
	{
		auto transform = Binding::BindingHelper::GetNativeObj<::Transform*>(obj);
		return transform->scale;
	}

	void Internal_SetScale(MonoObject* obj, ::Vector3 value)
	{
		auto transform = Binding::BindingHelper::GetNativeObj<::Transform*>(obj);
		transform->scale = value;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Transform::Internal_Create", reinterpret_cast<void*>(Create));
		mono_add_internal_call("ZaxEngine.Transform::get_position", reinterpret_cast<void*>(Internal_GetPosition));
		mono_add_internal_call("ZaxEngine.Transform::set_position", reinterpret_cast<void*>(Internal_SetPosition));
		mono_add_internal_call("ZaxEngine.Transform::get_rotation", reinterpret_cast<void*>(Internal_GetRotation));
		mono_add_internal_call("ZaxEngine.Transform::set_rotation", reinterpret_cast<void*>(Internal_SetRotation));
		mono_add_internal_call("ZaxEngine.Transform::get_scale", reinterpret_cast<void*>(Internal_GetScale));
		mono_add_internal_call("ZaxEngine.Transform::set_scale", reinterpret_cast<void*>(Internal_SetScale));
	}
}
