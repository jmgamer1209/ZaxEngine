#pragma once
#include <string>

class Application
{
private:
	Application() {};
	~Application() {};

public:
	static std::string contentPath;
	static int viewportWidth;
	static int viewportHeight;
	static bool isViewportSizeChanged;
	static bool isInMinimal;
};