#pragma once
#include "ThirdParty/glm/glm.hpp"

struct Vector3
{
	float x;
	float y;
	float z;

public:
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float* FloatPTR()
	{
		return (float*)this;
	}
};

glm::vec3 Vector3ToGLMVec(const Vector3& vec);

void GLMVecToVector3(const glm::vec3& glmVec, Vector3& vec);