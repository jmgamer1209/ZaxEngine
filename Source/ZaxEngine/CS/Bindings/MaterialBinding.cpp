#include "MeshBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "Core/ZString.h"
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

	
	void Internal_SetProperty_Texture(::Material* mat, MonoString* name, ::Texture* value)
	{
		/*auto mat = (::Material*)matPtr;
		auto value = (::Texture*)valuePtr;*/

		auto u8name = ZString(mono_string_to_utf16(name)).GetU8String();
		mat->SetProperty(u8name, MaterialProperty(value));
	}

	void Internal_SetProperty_Float(::Material* mat, MonoString* name, float value)
	{
		auto u8name = ZString(mono_string_to_utf16(name)).GetU8String();
		mat->SetProperty(u8name, MaterialProperty(value));
	}

	void Internal_SetProperty_Int(::Material* mat, MonoString* name, int32_t value)
	{
		auto u8name = ZString(mono_string_to_utf16(name)).GetU8String();
		mat->SetProperty(u8name, MaterialProperty(value));
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Material::Internal_Create_WithShader", reinterpret_cast<void*>(Internal_Create_WithShader));
		mono_add_internal_call("ZaxEngine.Material::Internal_SetProperty_Texture", reinterpret_cast<void*>(Internal_SetProperty_Texture));
		mono_add_internal_call("ZaxEngine.Material::Internal_SetProperty_Float", reinterpret_cast<void*>(Internal_SetProperty_Float));
		mono_add_internal_call("ZaxEngine.Material::Internal_SetProperty_Int", reinterpret_cast<void*>(Internal_SetProperty_Int));
	}
}
