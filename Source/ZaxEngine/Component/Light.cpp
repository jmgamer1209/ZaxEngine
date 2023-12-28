#include "Light.h"
#include "imgui.h"

void Light::OnGui()
{
	if (ImGui::TreeNode("Light")) {
		ImGui::Text("LightColor:");
		ImGui::ColorEdit3("##LightColor", this->color);
		ImGui::TreePop();
	}
}