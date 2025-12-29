#include "Input.h"
#include <GLFW/glfw3.h>
#include <Core/Application.h>

bool ZaxEngine::InputSystem::Input::GetKeyDown(int key)
{
	if (glfwGetKey(Application::window->window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

bool ZaxEngine::InputSystem::Input::GetMouseDown(int key)
{
	if (glfwGetMouseButton(Application::window->window, key) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}