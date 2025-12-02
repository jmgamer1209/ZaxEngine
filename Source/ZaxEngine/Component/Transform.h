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
	Vector3 GetForward();
	Vector3 GetRight();
	Vector3 GetUp();
};