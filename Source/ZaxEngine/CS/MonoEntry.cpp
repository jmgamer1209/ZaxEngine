#include "MonoEntry.h"
#include <iostream>
#include "boost/filesystem/path.hpp"
#include "Core/Debug.h"
#include "Core/Application.h"
#include "Core/Utils.h"
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/exception.h>

//std::cout << LOG << std::endl;	\

#define ASSERT_LOG_RETURN(A, NUM, LOG) if (!(A)) \
	{ \
		Debug::Log(LOG);	\
		return NUM; \
	}

MonoEntry MonoEntry::Instance;


void on_unhandled_exception(MonoObject* exc, void* user_data) {
	MonoString* exc_msg = mono_object_to_string(exc, NULL);
	Debug::Log(mono_string_to_utf8(exc_msg));
}

int MonoEntry::Init(boost::filesystem::path assemblies_search_path)
{
	// 设置程序集搜索目录
	mono_set_assemblies_path(assemblies_search_path.string().c_str());
	//mono_set_dirs(assemblies_search_path.string().c_str(), "");
	
	this->domain = mono_jit_init("TestMono");
	ASSERT_LOG_RETURN(domain != nullptr, 1, "error: domain");

	//注册全局异常回调
	mono_install_unhandled_exception_hook(on_unhandled_exception, NULL);
	return 0;
}

int MonoEntry::LoadAssembly(boost::filesystem::path assemblyPath, MonoAssembly** assembly, MonoImage **image)
{
	// 第二个参数填入要加载的c#的il
	MonoAssembly* temp_assembly = mono_domain_assembly_open(this->domain, assemblyPath.string().c_str());   //"G:\\Develop\\Windows\\ClassLibrary1\\ClassLibrary1\\bin\\Debug\\net8.0\\ClassLibrar2.dll"
	ASSERT_LOG_RETURN(temp_assembly != nullptr, 1, "error: can not load assembly");
	*assembly = temp_assembly;

	MonoImage* temp_image = mono_assembly_get_image(temp_assembly);
	ASSERT_LOG_RETURN(temp_image != nullptr, 1, "error: image");
	*image = temp_image;
	
	return 0;
}

int MonoEntry::LoadProjectAssembly() {
	auto path = Application::projectFolderPath / "Binary" / Application::projectName.replace_extension(".dll");
	return this->LoadAssembly(path, &this->assembly, &this->image);
}

int MonoEntry::LoadEngineAssembly()
{
	auto path = Utils::GetExeDirectory() / "ZaxEngine.Core.dll";
	return this->LoadAssembly(path, &this->assembly_engine, &this->image_engine);
}

int MonoEntry::RunGameStart() 
{ 
	//获取 Class
	MonoClass* main_class = mono_class_from_name(this->image, "", "GameEntry");

	// 获取 Method
	MonoMethod* mono_method = mono_class_get_method_from_name(main_class, "GameStart", 0);

	//调用方法
	mono_runtime_invoke(mono_method, NULL, NULL, NULL);

	return 0;
}

