#include "ComponentBinding.h"
#include "mono/metadata/class.h"
#include "Component/Component.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::Component
{
	mono_bool GetEnabled(MonoObject* obj)
	{
		auto native = BindingHelper::GetNativeObj<::Component*>(obj);
		return native->enabled;
	}

	void SetEnabled(MonoObject* obj, mono_bool value)
	{
		auto native = BindingHelper::GetNativeObj<::Component*>(obj);
		native->enabled = value;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Component::get_enabled", reinterpret_cast<void*>(GetEnabled));
		mono_add_internal_call("ZaxEngine.Component::set_enabled", reinterpret_cast<void*>(SetEnabled));
	}
};
