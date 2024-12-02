#include "Application.h"

#include "Editor/Window/EditorWindow.h"

boost::filesystem::path Application::projectPath;
std::string Application::contentPath = "";
int Application::viewportWidth = 1080;
int Application::viewportHeight = 720;
bool Application::isViewportSizeChanged = false;
bool Application::isInMinimal = false;
bool Application::isRunning = false;
WindowBase* Application::window = nullptr;

void Application::OpenEditor()
{
    Application::window = dynamic_cast<WindowBase*>(new EditorWindow());
}
