#include "Transform.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

Transform::Transform():position(0,0,0),rotation(0,0,0),scale(1,1,1)
{

}

void Transform::OnGui()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::Text("Position: ");
		ImGui::DragFloat3("##Position", this->position.FloatPTR(), 0.1f);

		ImGui::Text("Rotation: ");
		ImGui::DragFloat3("##Rotation", this->rotation.FloatPTR(), 0.1f, -180.0f, 180.0f);

		ImGui::Text("Scale: ");
		ImGui::DragFloat3("##Scale", this->scale.FloatPTR(), 0.1f);
		ImGui::TreePop();
	}
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