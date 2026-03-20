#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "Core/Vector.h"

class Transform : public Component
{
public:
	Vector3 position;
	// 欧拉角，旋转方向为内旋 y->x->z
	Vector3 rotation;
	Vector3 scale;

	Transform();
	Vector3 GetForward();
	Vector3 GetRight();
	Vector3 GetUp();
	glm::mat4 GetModelMatrix();
};