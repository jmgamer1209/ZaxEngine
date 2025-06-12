#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Core/Mesh.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

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

		Binding::BindingHelper::SetNativePtr(obj, nativeObj);

		// 测试代码，注释
		/*MonoClass* main_class = mono_class_from_name(MonoEntry::GetInstance()->image_engine, "ZaxEngine", "Mesh");
		MonoClassField* field = mono_class_get_field_from_name(main_class, "nativePtr");
		::Mesh* mesh = nullptr;
		mono_field_get_value(obj, field, &mesh);*/
			
		Debug::Log("Create Mesh Success");
	}

	MonoObject* Internal_GetQuadMesh() 
	{
		auto native = ::Mesh::GetQuadMesh();
		auto obj = Binding::BindingHelper::NewMonoObject("ZaxEngine", "Mesh");
		Binding::BindingHelper::SetNativePtr(obj, native);
		return obj;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Mesh::Internal_Create_Path", reinterpret_cast<void*>(CreateWithPath));
		mono_add_internal_call("ZaxEngine.Mesh::Internal_Create", reinterpret_cast<void*>(Create));
		mono_add_internal_call("ZaxEngine.Mesh::Internal_GetQuadMesh", reinterpret_cast<void*>(Internal_GetQuadMesh));
	}
}
