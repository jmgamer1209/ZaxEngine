#pragma once

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "mono/metadata/object.h"
#include "Core/ZString.h"
#include <string>

namespace ZaxEngine::Binding::BindingHelper {

	void SetNativePtr(MonoObject* obj, void* value);

	MonoObject* NewMonoObject(const char* name_space, const char* name);

	void MonoObjectSetValue(MonoObject* obj, const char* fieldName, void* value);

	template<typename T>
	T MonoObjectGetValue(MonoObject* obj, const char* fieldName) {
		auto mono_class = mono_object_get_class(obj);
		auto mono_field = mono_class_get_field_from_name(mono_class, fieldName);
		T value;
		//void* value = nullptr;
		mono_field_get_value(obj, mono_field, &value);
		return value;
	}

	std::string StringFromMonoString(MonoString* str);

	template<typename T>
	T GetNativeObj(MonoObject* obj) 
	{
		T native = MonoObjectGetValue<T>(obj, "nativePtr");
		return native;
	}

	bool IsSubOfGameComponent(MonoObject* obj);
}


