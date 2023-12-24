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
	ImGui::Text("Position: "); //ImGui::SameLine();
	ImGui::SliderFloat3("##Position", this->position, -20, 20);

	ImGui::Text("Rotation: "); //ImGui::SameLine();
	ImGui::SliderFloat3("##Rotation", this->rotation, 0, 90);

	ImGui::Text("Scale: ");
	ImGui::SliderFloat3("##Scale", this->scale, 0, 10);
}