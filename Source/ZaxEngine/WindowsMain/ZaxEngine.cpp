#include "ZaxEngine.h"
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


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	GLFW_INIT
	
	Application::window = dynamic_cast<WindowBase*>(new OpenWindow());
	Application::isRunning = true;

	setvbuf(stdout, nullptr, _IONBF, 0); //让控制台立即输出
	
	while (Application::isRunning)
	{
		Application::window->Update();
	}

	

	glfwTerminate();
	return 0;
}
