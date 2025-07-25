#include "GameComponentBinding.h"
#include "mono/metadata/class.h"
#include "Component/Component.h"
#include "BindingHelper.h"
#include <Core/Debug.h>
#include <mono/jit/jit.h>
#include "Scripting/ScriptFunc.h"

namespace ZaxEngine::Binding::GameComponent
{
	void Internal_Create(MonoObject* obj)
	{
		auto native = new ::Component();
		Binding::BindingHelper::SetNativePtr(obj, native);
		native->scriptObj = obj;

	/*	auto objClass = mono_object_get_class(obj);
		auto name_space = mono_class_get_namespace(objClass);
		auto name = mono_class_get_name(objClass);*/
		
		native->updateFunc = BindingHelper::GetUpdateFunc(obj);

		Debug::Log("Create Custom Component Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.GameComponent::Internal_Create", reinterpret_cast<void*>(Internal_Create));
	}
};
