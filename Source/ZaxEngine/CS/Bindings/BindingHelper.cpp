#include "BindingHelper.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "CS/MonoEntry.h"

namespace ZaxEngine::Binding::BindingHelper {

	void SetNativePtr(MonoObject* obj, void* value)
	{
		auto mono_class = mono_object_get_class(obj);
		MonoClassField* field = mono_class_get_field_from_name(mono_class, "nativePtr");
		mono_field_set_value(obj, field, &value);
	}
	
	MonoObject* NewMonoObject(const char* name_space, const char* name) {
		MonoClass* mono_class = mono_class_from_name(MonoEntry::GetInstance()->image_engine, name_space, name);
		return mono_object_new(MonoEntry::GetInstance()->domain, mono_class);
	}

	void MonoObjectSetValue(MonoObject* obj, const char* fieldName, void* value) {
		auto mono_class = mono_object_get_class(obj);
		auto mono_field = mono_class_get_field_from_name(mono_class, fieldName);
		mono_field_set_value(obj, mono_field, value);
	}

	string StringFromMonoString(MonoString* str)
	{
		return ZString(mono_string_to_utf16(str)).GetU8String();
	}
}
