#include "ZaxEngine.h"
#include <iostream>
#include "Core/ShaderProgram.h"
#include "Core/Debug.h"
#include "Core/Utils.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Component/Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Core/GameObject.h"
#include "Core/Application.h"
#include "Core/WindowBase.h"
#include "Editor/Window/EditorWindow.h"
#include "Editor/Window/OpenWindow.h"
#include "filesystem/path.hpp"
#include "filesystem/directory.hpp"
#include "filesystem/operations.hpp"
#include "CS/MonoEntry.h"


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// 添加 dll 搜索路径，这里需要导出后 dll 的位置，当前关联了项目设置的生成后事件的dll复制
	filesystem::path exePath = Utils::GetExeDirectory();
	auto dataPath = exePath / L"Data";
	auto monoPath = dataPath / L"Mono";
	auto standardPath = dataPath / L"Mono" / L"net_4_x-win32";
	SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
	AddDllDirectory(dataPath.wstring().c_str());
	AddDllDirectory(monoPath.wstring().c_str());
	AddDllDirectory(standardPath.wstring().c_str());
	
	setvbuf(stdout, nullptr, _IONBF, 0); //设置控制台立即输出

	GLFW_INIT
	
	Application::window = dynamic_cast<WindowBase*>(new OpenWindow());
	Application::isRunning = true;

	auto assemblies_path = monoPath.string() + ";" + standardPath.string(); // windows使用; 类Unix使用:，这里先只适配 windows
	MonoEntry::GetInstance()->Init(assemblies_path);

	
	while (Application::isRunning)
	{
		Application::window->Update();
	}

	

	glfwTerminate();
	return 0;
}
