#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Core/Mesh.h"
#include "CS/MonoEntry.h"

namespace ZaxEngine::Binding::Mesh {

	void Create(MonoObject* obj)
	{
		auto nativeObj = new ::Mesh();
		Debug::Log("Create Mesh Success");
	}

	void CreateWithPath(MonoObject* obj, MonoString* path)
	{
		auto wcharPath = mono_string_to_utf16(path);
		auto nativeObj = new ::Mesh(wcharPath);
		
		MonoClass* main_class = mono_class_from_name(MonoEntry::GetInstance()->image_engine, "ZaxEngine", "Mesh");
		MonoClassField* field = mono_class_get_field_from_name(main_class, "nativePtr");
		mono_field_set_value(obj, field, nativeObj);
		//mono_property_set_value(property,)
			
		Debug::Log("Create Mesh Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Mesh::Internal_Create_Path", reinterpret_cast<void*>(CreateWithPath));
		mono_add_internal_call("ZaxEngine.Mesh::Internal_Create", reinterpret_cast<void*>(Create));
	}
}
