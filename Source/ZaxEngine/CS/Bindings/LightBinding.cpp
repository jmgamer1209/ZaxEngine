#include "LightBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Component/Light.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::Light {

	void Internal_Create_WithType(MonoObject* obj, LightType lightType)
	{
		auto nativeObj = new ::Light(lightType);
		Binding::BindingHelper::SetNativePtr(obj, nativeObj);
		nativeObj->scriptObj = obj;
			
		Debug::Log("Create Light Success");
	}

	::Color Internal_GetColor(MonoObject* obj)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		return native->color;
	}

	void Internal_SetColor(MonoObject* obj, ::Color value)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		native->color = value;
	}

	float Internal_GetRange(MonoObject* obj)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		return native->range;
	}

	void Internal_SetRange(MonoObject* obj, float value)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		native->range = value;
	}

	float Internal_GetInnerAngle(MonoObject* obj)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		return native->innerAngle;
	}

	void Internal_SetInnerAngle(MonoObject* obj, float value)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		native->innerAngle = value;
	}

	float Internal_GetOuterAngle(MonoObject* obj)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		return native->outerAngle;
	}

	void Internal_SetOuterAngle(MonoObject* obj, float value)
	{
		auto native = Binding::BindingHelper::GetNativeObj<::Light*>(obj);
		native->outerAngle = value;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Light::Internal_Create_WithType", reinterpret_cast<void*>(Internal_Create_WithType));
		mono_add_internal_call("ZaxEngine.Light::get_color", reinterpret_cast<void*>(Internal_GetColor));
		mono_add_internal_call("ZaxEngine.Light::set_color", reinterpret_cast<void*>(Internal_SetColor));
		mono_add_internal_call("ZaxEngine.Light::get_range", reinterpret_cast<void*>(Internal_GetRange));
		mono_add_internal_call("ZaxEngine.Light::set_range", reinterpret_cast<void*>(Internal_SetRange));
		mono_add_internal_call("ZaxEngine.Light::get_innerAngle", reinterpret_cast<void*>(Internal_GetInnerAngle));
		mono_add_internal_call("ZaxEngine.Light::set_innerAngle", reinterpret_cast<void*>(Internal_SetInnerAngle));
		mono_add_internal_call("ZaxEngine.Light::get_outerAngle", reinterpret_cast<void*>(Internal_GetOuterAngle));
		mono_add_internal_call("ZaxEngine.Light::set_outerAngle", reinterpret_cast<void*>(Internal_SetOuterAngle));
	}
}
