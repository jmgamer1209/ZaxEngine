#pragma once
#include <string>

#include "WindowBase.h"

class Application
{
private:
	Application() {}
	~Application() = default;

public:
	static std::string contentPath;
	static int viewportWidth;
	static int viewportHeight;
	static bool isViewportSizeChanged;
	static bool isInMinimal;
	static bool isRunning;
	static WindowBase* window;

	static void OpenEditor();
};
