#pragma once
#include <mono/metadata/object.h>
#include <string>

namespace ZaxEngine::Binding::Application
{
	void TriggerStaticInit();
	void SetContentPath(std::string path);
}