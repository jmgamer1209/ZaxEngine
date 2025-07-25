#include "SceneBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Scene.h"
#include "Core/Debug.h"
#include "Core/SceneManager.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::Scene {

	void CreateScene(MonoObject* obj) 
	{
		auto scene = new ::Scene();
		SceneManager::mainScene = scene;
		BindingHelper::SetNativePtr(obj, scene);
		Debug::Log("Create Scene Success");
	}

	void Internal_AddGameObject(MonoObject* obj, MonoObject* go)
	{
		auto native = BindingHelper::GetNativeObj<::Scene*>(obj);
		auto nativeGO = BindingHelper::GetNativeObj<::GameObject*>(go);
		native->AddGameObject(nativeGO);
	}

	void RegisterInternalCall() 
	{
		mono_add_internal_call("ZaxEngine.Scene::Internal_Create", reinterpret_cast<void*>(CreateScene));
		mono_add_internal_call("ZaxEngine.Scene::Internal_AddGameObject", reinterpret_cast<void*>(Internal_AddGameObject));
	}
}
