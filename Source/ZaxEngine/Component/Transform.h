#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class Transform : public Component
{
public:
	float position[3] = { 0, 0, 0 };
	float rotation[3] = { 0, 0, 0 };
	float scale[3] = { 1,1,1 };

	void OnGui() override;
	glm::vec3 GetForward();
};