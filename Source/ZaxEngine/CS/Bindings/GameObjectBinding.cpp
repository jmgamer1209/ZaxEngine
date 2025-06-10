#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Core/GameObject.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::GameObject {

	void Create(MonoObject* obj)
	{
		//auto nativeObj = new ::ShaderProgram();
		//Debug::Log("Create Mesh Success");
	}

	void CreateWithName(MonoObject* obj, MonoString* name)
	{
		auto nativeObj = new ::GameObject(Binding::BindingHelper::StringFromMonoString(name));
		Binding::BindingHelper::SetNativePtr(obj, nativeObj);
		Debug::Log("Create GameObject Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.GameObject::Internal_CreateWithName", reinterpret_cast<void*>(CreateWithName));
		mono_add_internal_call("ZaxEngine.GameObject::Internal_Create", reinterpret_cast<void*>(Create));
	}
}
