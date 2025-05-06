#include "WindowsMain/MonoEntry.h"
#include <iostream>
#include "boost/filesystem/path.hpp"
#include "Core/Debug.h"
//std::cout << LOG << std::endl;	\

#define ASSERT_LOG_RETURN(A, NUM, LOG) if (!(A)) \
	{ \
		Debug::Log(LOG);	\
		return NUM; \
	}

MonoEntry MonoEntry::Instance;

int MonoEntry::Init(boost::filesystem::path assemblies_search_path)
{
	// 设置程序集搜索目录
	//mono_set_assemblies_path
	mono_set_dirs(assemblies_search_path.string().c_str(), "");
	
	this->domain = mono_jit_init("TestMono");
	ASSERT_LOG_RETURN(domain != nullptr, 1, "error: domain");
	return 0;
}

int MonoEntry::LoadAssembly(boost::filesystem::path assemblyPath)
{
	// 以下为测试程序，这里要改为 C# 脚本项目
	// 第二个参数填入要加载的c#的il
	MonoAssembly* assembly = mono_domain_assembly_open(this->domain, assemblyPath.string().c_str());   //"G:\\Develop\\Windows\\ClassLibrary1\\ClassLibrary1\\bin\\Debug\\net8.0\\ClassLibrar2.dll"
	ASSERT_LOG_RETURN(assembly != nullptr, 1, "error: can not load assembly");

	MonoImage* image = mono_assembly_get_image(assembly);
	ASSERT_LOG_RETURN(image != nullptr, 1, "error: image");

	MonoClass* main_class = mono_class_from_name(image, "Test", "Class1");
	ASSERT_LOG_RETURN(image != nullptr, 1, "error: main_class");

	MonoMethod* mono_method = mono_class_get_method_from_name(main_class, "Fun", 0);
	ASSERT_LOG_RETURN(mono_method != nullptr, 1, "error: main_class");
	mono_runtime_invoke(mono_method, nullptr, nullptr, nullptr);
	return 0;
}
