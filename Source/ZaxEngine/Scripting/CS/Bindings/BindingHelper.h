#pragma once

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "mono/metadata/object.h"
#include "Core/ZString.h"
#include <string>
#include <vector>
#include <Scripting/ScriptField.h>

namespace ZaxEngine::Binding::BindingHelper {

	void SetNativePtr(MonoObject* obj, void* value);

	MonoObject* NewMonoObject(const char* name_space, const char* name);

	void MonoObjectSetValue(MonoObject* obj, const char* fieldName, void* value);
	void MonoObjectSetValue(MonoObject* obj, MonoClassField* field, void* value);

	template<typename T>
	T MonoObjectGetValue(MonoObject* obj, const char* fieldName) {
		auto mono_class = mono_object_get_class(obj);
		auto mono_field = mono_class_get_field_from_name(mono_class, fieldName);
		T value;
		//void* value = nullptr;
		mono_field_get_value(obj, mono_field, &value);
		return value;
	}

	template<typename T>
	T MonoObjectGetValue(MonoObject* obj, MonoClassField* field) {
		T value;
		//void* value = nullptr;
		mono_field_get_value(obj, field, &value);
		return value;
	}

	std::string StringFromMonoString(MonoString* str);
	std::wstring WStringFromMonoString(MonoString* str);

	template<typename T>
	T GetNativeObj(MonoObject* obj) 
	{
		// 编译期检查 T 是否为指针类型，否则报错
		static_assert(std::is_pointer_v<T>, "模板参数 T 必须是指针类型！例如 GetNativeObj<Quat*>(obj)");
		T native = MonoObjectGetValue<T>(obj, "nativePtr");
		return native;
	}

	bool IsSubOfGameComponent(MonoObject* obj);

	MonoMethod* GetUpdateFunc(MonoObject* obj);

	MonoObject* MonoInvoke(MonoMethod* method, void* obj, void** params);

	const char* GetTypeName(MonoObject* obj);
	std::vector<Scripting::ScriptField> GetAllFields(MonoObject* obj);
}


