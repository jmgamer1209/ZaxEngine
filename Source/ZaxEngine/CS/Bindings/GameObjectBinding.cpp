#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Core/GameObject.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "Component/Transform.h"
#include <mono/metadata/reflection.h>

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

	void AddComponent(MonoObject* obj, ::GameObject* go, Component* comp)
	{
		go->AddComponent(comp);
	}

	MonoObject* Internal_GetComponent(MonoObject*obj, MonoReflectionType* reflectionType)
	{
		auto go = BindingHelper::GetNativeObj<::GameObject*>(obj);

		// 获取 MonoType 对象
		MonoType* type = mono_reflection_type_get_type(reflectionType);
		auto typeClass = mono_type_get_class(type);
		::Component* target = nullptr;

		for (size_t i = 0; i < go->components.size(); i++)
		{
			auto objClass = go->components[i]->scriptObj.monoClass;
			if (typeClass == objClass) 
			{
				target = go->components[i];
				break;
			}
		}

		//// 获取类型类
		//MonoClass* typeClass = mono_class_from_mono_type(type);

		//// 获取类型名称
		//const char* name = mono_class_get_name(typeClass);

		//// 获取命名空间
		//const char* namespaceName = mono_class_get_namespace(typeClass);

		//// 输出完整类型名
		////std::cout << "Type: " << namespaceName << "." << name << std::endl;

		//

		//auto component = go->GetComponent<::Transform>();
		auto monoobj = BindingHelper::NewMonoObject("ZaxEngine", "Transform");
		BindingHelper::SetNativePtr(monoobj, target);
		return monoobj;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.GameObject::Internal_CreateWithName", reinterpret_cast<void*>(CreateWithName));
		mono_add_internal_call("ZaxEngine.GameObject::Internal_Create", reinterpret_cast<void*>(Create));
		mono_add_internal_call("ZaxEngine.GameObject::Internal_AddComponent", reinterpret_cast<void*>(AddComponent));
		mono_add_internal_call("ZaxEngine.GameObject::Internal_GetComponent", reinterpret_cast<void*>(Internal_GetComponent));
	}
}
