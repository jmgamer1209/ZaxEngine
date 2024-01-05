#include "Vector3.h"

glm::vec3 Vector3ToGLMVec(const Vector3& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

void GLMVecToVector3(const glm::vec3& glmVec, Vector3& vec)
{
	vec.x = glmVec.x;
	vec.y = glmVec.y;
	vec.z = glmVec.z;
}