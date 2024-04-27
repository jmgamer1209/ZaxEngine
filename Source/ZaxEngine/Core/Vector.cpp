#include "Core/Vector.h"

glm::vec3 Vector3ToGLMVec(const Vector3& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

Vector3 Vector3::operator +(const Vector3& other)
{
	Vector3 v;
	v.x = this->x + other.x;
	v.y = this->y + other.y;
	v.z = this->z + other.z;
	return v;
}

Vector3 Vector3::operator -(const Vector3& other)
{
	Vector3 v;
	v.x = this->x - other.x;
	v.y = this->y - other.y;
	v.z = this->z - other.z;
	return v;
}

Vector3 Vector3::operator -()
{
	return Vector3(-this->x, -this->y, -this->z);
}

glm::vec3 Vector3::ToGLMVec()
{
	glm::vec3 v;
	v.x = this->x;
	v.y = this->y;
	v.z = this->z;
	return v;
}

void GLMVecToVector3(const glm::vec3& glmVec, Vector3& vec)
{
	vec.x = glmVec.x;
	vec.y = glmVec.y;
	vec.z = glmVec.z;
}