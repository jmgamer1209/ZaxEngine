#include "BindingHelper.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "CS/MonoEntry.h"

namespace ZaxEngine::Binding::BindingHelper {

	void SetNativePtr(MonoObject* obj, void* value, const char* name_space, const char* name)
	{
		MonoClass* main_class = mono_class_from_name(MonoEntry::GetInstance()->image_engine, name_space, name);
		MonoClassField* field = mono_class_get_field_from_name(main_class, "nativePtr");
		mono_field_set_value(obj, field, &value);
	}
}
