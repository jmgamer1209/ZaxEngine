#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "GameObject.h"

class Camera: public Component
{
public:
	
	float cameraNear = 0.1f;
	float cameraFar = 100.0f;
	glm::vec3 cameraFront;
	float fovy = glm::radians(45.0f);
	int viewportWidth;
	int viewportHeight;

public:
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjection();
	void HandleCameraInput(GLFWwindow* window);
	void OnCursorPosChange(float xOffset, float yOffset);
	void OnViewportChange(int width, int height);
};

