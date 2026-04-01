#pragma once
#include <string>

#include "WindowBase.h"
#include "Editor/ProjectConfig.h"
#include "filesystem/path.hpp"
#include "Renderer/SceneRenderer.h"

class Application
{
private:
	Application() {}
	~Application() = default;

public:
	static boost::filesystem::path projectPath; // zproject 文件
	static boost::filesystem::path projectName; // 项目名
	static boost::filesystem::path projectFolderPath; // zproject 文件所在的目录
	static boost::filesystem::path contentPath;
	static ProjectConfig projectConfig;
	static int viewportWidth;
	static int viewportHeight;
	static bool isViewportSizeChanged;
	static bool isInMinimal;
	static bool isRunning;
	static WindowBase* window;
	static SceneRenderer* sceneRenderer;

	static void OpenEditor(const boost::filesystem::path& projectPath);
};
