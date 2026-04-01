#include "TextureBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "CS/MonoEntry.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "Core/ZString.h"

namespace ZaxEngine::Binding::Texture {

	//void Create(MonoObject* obj)
	//{
	//	auto nativeObj = new ::Mesh();
	//	Debug::Log("Create Mesh Success");
	//}

	MonoObject* Internal_Load(MonoString* path, TextureType type, bool flip_vertically)
	{
		auto mPath = mono_string_to_utf16(path);
		//先转为 ZString，规范编码
		auto str = ZString(mPath);
		auto u8Str = str.GetU8String();
		auto tex = ::Texture::Load(u8Str, type, flip_vertically);
		
		MonoObject* monoTex = BindingHelper::NewMonoObject(BindingCommon::EngineNameSpace, "Texture");

		BindingHelper::MonoObjectSetValue(monoTex, "ID", &tex->ID);
		BindingHelper::MonoObjectSetValue(monoTex, "type", &tex->type);
		BindingHelper::SetNativePtr(monoTex, tex);

		//auto id = BindingHelper::MonoObjectGetValue<unsigned int>(monoTex, "ID");
		//auto mtype = BindingHelper::MonoObjectGetValue<TextureType>(monoTex, "type");

		return monoTex;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Texture::Internal_Load", reinterpret_cast<void*>(Internal_Load));
	}
}
