#pragma once
#include <mono/metadata/object.h>

namespace ZaxEngine::Binding::ShaderProgram
{
	void Create(MonoObject* obj);
	void CreateWithPath(MonoObject* obj, MonoString* vertexPath, MonoString* fragmentPath);

	void RegisterInternalCall();
}