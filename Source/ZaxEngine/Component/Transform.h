#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "Core/Vector.h"

class Transform : public Component
{
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform();
	void OnGui() override;
	glm::vec3 GetForward();
};