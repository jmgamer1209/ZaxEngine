#include "GameObject.h"
#include "imgui/imgui.h"

GameObject::GameObject(const string& name)
{
	this->name = name;
}

void GameObject::AddComponent(Component* component)
{
	component->gameObject = this;
	components.push_back(component);
}

void Transform::OnGui()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::Text("Position: "); 
		ImGui::DragFloat3("##Position", this->position, 1.0f);

		ImGui::Text("Rotation: ");
		ImGui::DragFloat3("##Rotation", this->rotation, 1.0f, -180.0f, 180.0f);

		ImGui::Text("Scale: ");
		ImGui::DragFloat3("##Scale", this->scale, 0.1f);
		ImGui::TreePop();
	}
}

glm::vec3 Transform::GetForward()
{
	glm::mat4 modelRotation(1.0f);
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation[1]), glm::vec3(0, 1, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation[0]), glm::vec3(1, 0, 0));
	modelRotation = glm::rotate(modelRotation, glm::radians(rotation[2]), glm::vec3(0, 0, 1));
	return modelRotation * glm::vec4(0, 0, -1, 1);
}