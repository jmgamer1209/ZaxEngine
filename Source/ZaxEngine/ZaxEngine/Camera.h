#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	float position[3] = { 0, 0, 3 };
	float pitch = 0;
	float yaw = -90.0f;
	float cameraNear = 0.1f;
	float cameraFar = 100.0f;
	glm::vec3 cameraFront;
	float fovy = glm::radians(45.0f);
	int viewportWidth;
	int viewportHeight;

public:
	glm::mat4 GetLookAt();
	glm::mat4 GetProjection();
	void HandleCameraInput(GLFWwindow* window);
	void OnCursorPosChange(float xOffset, float yOffset);
};

