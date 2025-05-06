#include "Application.h"

#include "Editor/Window/EditorWindow.h"

boost::filesystem::path Application::projectPath;
boost::filesystem::path Application::projectName;
boost::filesystem::path Application::projectFolderPath;
ProjectConfig Application::projectConfig;
boost::filesystem::path Application::contentPath;
int Application::viewportWidth = 1080;
int Application::viewportHeight = 720;
bool Application::isViewportSizeChanged = false;
bool Application::isInMinimal = false;
bool Application::isRunning = false;
WindowBase* Application::window = nullptr;

void Application::OpenEditor(const boost::filesystem::path& projectPath)
{
    Application::projectPath = projectPath;
    Application::projectFolderPath = projectPath.parent_path();
    Application::projectName = projectPath.stem().string();
    contentPath = (Application::projectFolderPath / ("Content")).string();
    window = dynamic_cast<WindowBase*>(new EditorWindow());
}
