#include "ApplicationBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include <CS/MonoEntry.h>
//#include "mono/utils/mono-logger.h"

namespace ZaxEngine::Binding::Application
{
	void TriggerStaticInit() 
	{
		MonoClass* mclass = mono_class_from_name(MonoEntry::GetInstance()->image_engine, "ZaxEngine", "Application");
		auto method = mono_class_get_method_from_name(mclass, "TriggerStaticInit", 0);
		mono_runtime_invoke(method, nullptr, nullptr, nullptr);
	}

	void SetContentPath(std::string path) 
	{
		//mono_trace_set_level_string("debug");
		//mono_trace_set_level(MONO_TRACE_DEBUG);

		MonoClass* mclass = mono_class_from_name(MonoEntry::GetInstance()->image_engine, "ZaxEngine", "Application");
		auto vtable = mono_class_vtable(MonoEntry::GetInstance()->domain, mclass);
		MonoClassField* field = mono_class_get_field_from_name(mclass, "contentPath");

		MonoString* value = mono_string_new(MonoEntry::GetInstance()->domain, path.c_str());
		
		mono_field_static_set_value(vtable, field, value);
	}
}