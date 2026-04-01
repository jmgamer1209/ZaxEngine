#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "Core/ShaderProgram.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::ShaderProgram {

	void Create(MonoObject* obj)
	{
		//auto nativeObj = new ::ShaderProgram();
		//Debug::Log("Create Mesh Success");
	}

	void CreateWithPath(MonoObject* obj, MonoString* vertexPath, MonoString* fragmentPath)
	{
		auto wcharVPath = mono_string_to_utf16(vertexPath);
		auto wcharFPath = mono_string_to_utf16(fragmentPath);
		auto nativeObj = new ::ShaderProgram(wcharVPath, wcharFPath);

		Binding::BindingHelper::SetNativePtr(obj, nativeObj);
			
		Debug::Log("Create ShaderProgram Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.ShaderProgram::Internal_Create_Path", reinterpret_cast<void*>(CreateWithPath));
		mono_add_internal_call("ZaxEngine.ShaderProgram::Internal_Create", reinterpret_cast<void*>(Create));
	}
}
