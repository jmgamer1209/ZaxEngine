#pragma once

#include "ThirdParty/glm/glm.hpp"

struct Vector3
{
	float x;
	float y;
	float z;

public:
	Vector3()
	{
		x = y = z = 0;
	}

	Vector3(float v)
	{
		x = y = z = v;
	}

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

	Vector3 operator +(const Vector3& other);

	Vector3 operator -(const Vector3& other);

	glm::vec3 ToGLMVec();
};

glm::vec3 Vector3ToGLMVec(const Vector3& vec);

void GLMVecToVector3(const glm::vec3& glmVec, Vector3& vec);

struct Vector2
{
	float x;
	float y;

public:
	Vector2()
	{
	}
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float* FloatPTR()
	{
		return (float*)this;
	}
};