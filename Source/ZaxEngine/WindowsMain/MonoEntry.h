#pragma once
#include "boost/filesystem/path.hpp"
// mono header
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>

class MonoEntry
{	
private:
	MonoEntry() {}
public:
	MonoDomain* domain = nullptr;
	MonoAssembly* assembly = nullptr;
	MonoImage* image = nullptr;
	MonoAssembly* assembly_engine = nullptr;
	MonoImage* image_engine = nullptr;

	static MonoEntry Instance;
	static MonoEntry* GetInstance() {
		return &Instance;
	}

	int Init(boost::filesystem::path standardPath);
	int LoadAssembly(boost::filesystem::path assemblyPath, MonoAssembly** assembly, MonoImage** image);
	int LoadProjectAssembly();
	int LoadEngineAssembly();
};

