#pragma once
#include <string>

#include "WindowBase.h"
#include "filesystem/path.hpp"

class Application
{
private:
	Application() {}
	~Application() = default;

public:
	static boost::filesystem::path projectPath;
	static std::string contentPath;
	static int viewportWidth;
	static int viewportHeight;
	static bool isViewportSizeChanged;
	static bool isInMinimal;
	static bool isRunning;
	static WindowBase* window;

	static void OpenEditor();
};
