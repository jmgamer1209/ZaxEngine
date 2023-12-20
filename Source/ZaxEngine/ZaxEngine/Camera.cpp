#include "Camera.h"
#include "Utils.h"

glm::mat4 Camera::GetLookAt()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glm::vec3 pos = glm::vec3(position[0], position[1], position[2]);
	auto lookAt = glm::lookAt(pos,
		pos + cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f));
	return lookAt;
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
	yaw += (float)xOffset * 0.1f;
	pitch -= (float)yOffset * 0.1f;
}

void Camera::OnViewportChange(int width, int height)
{
	viewportWidth = width;
	viewportHeight = height;
}