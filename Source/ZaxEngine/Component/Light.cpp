#include "Light.h"
#include "imgui.h"

Light::Light(LightType type): range(10)
{
	this->type = type;
}

void Light::OnGui()
{
	if (ImGui::TreeNode("Light")) {
		ImGui::Text("LightType: %s", LightTypeToString(type).c_str());
		ImGui::Text("LightColor:");
		ImGui::ColorEdit3("##LightColor", color.FloatPTR());

		if (this->type == LightType::Point)
		{
			ImGui::Text("Range:");
			ImGui::DragFloat("##Range", &this->range, 0.1f, 0, FLT_MAX);
		}
		else if (this->type == LightType::Spot)
		{
			ImGui::Text("Range:");
			ImGui::DragFloat("##Range", &this->range, 0.1f, 0, FLT_MAX);
			ImGui::Text("Innter:");
			ImGui::DragFloat("##Inner", &this->innerAngle, 0.1f, 1, 179);
			ImGui::Text("Outer:");
			ImGui::DragFloat("##Outer", &this->outerAngle, 0.1f, 1, 179);
		}

		ImGui::Text("ShadowDepthBias:");
		ImGui::DragFloat("##ShadowDepthBias", &shadowDepthBias, 0.01, 0, 2);

		ImGui::TreePop();
	}
}

std::string LightTypeToString(LightType type)
{
	switch (type)
	{
	case LightType::Directional:
		return "Directional";
	case LightType::Point:
		return "Point";
	case LightType::Spot:
		return "Spot";
	default:
		return "No Type Name";
	}
}