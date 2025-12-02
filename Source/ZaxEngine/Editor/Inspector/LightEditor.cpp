#include "LightEditor.h"
#include <imgui.h>
#include <Component/Light.h>

namespace ZaxEngine::Editor
{
	LightEditor::LightEditor(Component* target): InspectorEditor(target)
	{
	}

	void LightEditor::OnGui()
	{
		auto light = (Light*)target;
		if (ImGui::TreeNode("Light")) {
			ImGui::Text("LightType: %s", LightTypeToString(light->type).c_str());
			ImGui::Text("LightColor:");
			ImGui::ColorEdit3("##LightColor", light->color.FloatPTR());

			if (light->type == LightType::Point)
			{
				ImGui::Text("Range:");
				ImGui::DragFloat("##Range", &light->range, 0.1f, 0, FLT_MAX);
			}
			else if (light->type == LightType::Spot)
			{
				ImGui::Text("Range:");
				ImGui::DragFloat("##Range", &light->range, 0.1f, 0, FLT_MAX);
				ImGui::Text("Innter:");
				ImGui::DragFloat("##Inner", &light->innerAngle, 0.1f, 1, 179);
				ImGui::Text("Outer:");
				ImGui::DragFloat("##Outer", &light->outerAngle, 0.1f, 1, 179);
			}

			ImGui::Text("ShadowDepthBias:");
			ImGui::DragFloat("##ShadowDepthBias", &light->shadowDepthBias, 0.01f, 0, 2);

			ImGui::TreePop();
		}
	}
}