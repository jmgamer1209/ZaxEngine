#include "Camera.h"
#include "Utils.h"

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 viewRotation(1.0f);
	viewRotation = glm::rotate(viewRotation, glm::radians(rotation[1]), glm::vec3(0, 1, 0));
	viewRotation = glm::rotate(viewRotation, glm::radians(rotation[0]), glm::vec3(1, 0, 0));
	viewRotation = glm::rotate(viewRotation, glm::radians(rotation[2]), glm::vec3(0, 0, 1));
	viewRotation = glm::transpose(viewRotation);

	glm::mat4 viewTranslation(1.0f);
	viewTranslation = glm::translate(viewTranslation, glm::vec3(-position[0], -position[1], -position[2]));

	return viewRotation * viewTranslation;
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(fovy, (float)viewportWidth / (float)viewportHeight, cameraNear, cameraFar);
}

void Camera::HandleCameraInput(GLFWwindow* window)
{
	float moveSpeed = 0.05f;
	glm::vec3 pos(position[0], position[1], position[2]);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		pos = pos + cameraFront * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		pos = pos - cameraFront * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		pos = pos - glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0))) * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		pos = pos + glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0))) * moveSpeed;
	}

	Utils::Vec3ToArray(pos, position);
}

void Camera::OnCursorPosChange(float xOffset, float yOffset)
{
	//yaw += (float)xOffset * 0.1f;
	//pitch -= (float)yOffset * 0.1f;
}

void Camera::OnViewportChange(int width, int height)
{
	viewportWidth = width;
	viewportHeight = height;
}