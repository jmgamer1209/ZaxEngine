#include "Camera.h"
#include "Utils.h"
#include "imgui/imgui.h"

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 viewRotation(1.0f);
	auto transform = gameObject->GetComponent<Transform>();
	auto position = transform->position;
	auto rotation = transform->rotation;

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
	return glm::perspective(glm::radians(fovy), (float)viewportWidth / (float)viewportHeight, clippingNear, clippingFar);
}

void Camera::HandleCameraInput(GLFWwindow* window)
{
	auto transform = gameObject->GetComponent<Transform>();
	float moveSpeed = 0.05f;
	glm::vec3 pos = Utils::ArrayToVec3(transform->position);
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

	Utils::Vec3ToArray(pos, transform->position);
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

void Camera::OnGui()
{
	if (ImGui::TreeNode("Camera")) {
		ImGui::SliderFloat("Fov-Vertical", &this->fovy, 0.1f, 179.0f);
		ImGui::InputFloat("Near", &this->clippingNear);
		ImGui::InputFloat("Far", &this->clippingFar);

		ImGui::TreePop();
	}
}