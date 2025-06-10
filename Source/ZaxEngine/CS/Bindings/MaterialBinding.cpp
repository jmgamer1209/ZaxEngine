#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "Core/Material.h"

namespace ZaxEngine::Binding::Material {

	//void Create(MonoObject* obj)
	//{
	//	auto nativeObj = new ::Mesh();
	//	Debug::Log("Create Mesh Success");
	//}

	void Internal_Create_WithShader(MonoObject* obj, intptr_t shaderPtr)
	{
		auto shader = (ShaderProgram*)shaderPtr;
		auto nativeObj = new ::Material(shader);

		Binding::BindingHelper::SetNativePtr(obj, nativeObj);
			
		Debug::Log("Create Mat Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Material::Internal_Create_WithShader", reinterpret_cast<void*>(Internal_Create_WithShader));
	}
}
