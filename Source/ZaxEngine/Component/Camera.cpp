#include "Component/Camera.h"
#include "Core/Utils.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Component/Transform.h"

glm::mat4 Camera::GetViewMatrix()
{
	return Utils::GetViewMatrix(*gameObject->GetComponent<Transform>());
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::radians(fovy), (float)viewportWidth / (float)viewportHeight, clippingNear, clippingFar);
}

void Camera::HandleCameraInput(GLFWwindow* window)
{
	auto transform = gameObject->GetComponent<Transform>();
	float moveSpeed = 0.05f;
	glm::vec3 pos = Vector3ToGLMVec(transform->position);
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
	GLMVecToVector3(pos, transform->position);
}

void Camera::OnCursorPosChange(float xOffset, float yOffset)
{
	//yaw += (float)xOffset * 0.1f;
	//pitch -= (float)yOffset * 0.1f;
}

void Camera::OnViewportChange(int width, int height)
{
	if (width == 0 && height == 0) return;   // 当最小化时，这两者都变为0，如果继续渲染，则导致报错
	viewportWidth = width;
	viewportHeight = height;
}

void Camera::OnGui()
{
	if (ImGui::TreeNode("Camera")) {
		ImGui::Text("Fov-Vertical:");
		ImGui::DragFloat("##Fov-Vertical", &this->fovy, 1.0f, 0.1f, 179.0f);
		ImGui::Text("Near:");
		ImGui::DragFloat("##Near", &this->clippingNear, 1.0f, 0, (float)INT32_MAX);
		ImGui::Text("Far:");
		ImGui::DragFloat("##Far", &this->clippingFar, 1.0f, 0, (float)INT32_MAX);

		ImGui::TreePop();
	}
}