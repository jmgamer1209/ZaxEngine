#include "Transform.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

Transform::Transform():position(0,0,0),rotation(0,0,0),scale(1,1,1)
{

}

Vector3 Transform::GetForward()
{
	glm::mat4 modelRotation(1.0f);
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	
	Vector3 v1;
	glm::vec3 v2 = modelRotation * glm::vec4(0, 0, -1, 1);
	GLMVecToVector3(v2, v1);
	return v1;
}

Vector3 Transform::GetRight()
{
	glm::mat4 modelRotation(1.0f);
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	
	Vector3 v1;
	glm::vec3 v2 = modelRotation * glm::vec4(1, 0, 0, 1);
	GLMVecToVector3(v2, v1);
	return v1;
}

Vector3 Transform::GetUp()
{
	glm::mat4 modelRotation(1.0f);
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));

	Vector3 v1;
	glm::vec3 v2 = modelRotation * glm::vec4(0, 1, 0, 1);
	GLMVecToVector3(v2, v1);

	return v1;
}

glm::mat4 Transform::GetModelMat()
{
	glm::mat4 model(1.0f);
	// 位移
	model = glm::translate(model, Vector3ToGLMVec(this->position));
	// 旋转，此旋转构造是基于模型本身的局部坐标轴
	model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
	// 缩放
	model = glm::scale(model, Vector3ToGLMVec(this->scale));
	
	return model;
}
