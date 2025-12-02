#include "TransformEditor.h"
#include <imgui.h>
#include <Component/Transform.h>

namespace ZaxEngine::Editor
{
	TransformEditor::TransformEditor(Component* target): InspectorEditor(target)
	{
	}

	void TransformEditor::OnGui()
	{
		auto component = (Transform*)target;
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::Text("Position: ");
			ImGui::DragFloat3("##Position", component->position.FloatPTR(), 0.1f);

			ImGui::Text("Rotation: ");
			ImGui::DragFloat3("##Rotation", component->rotation.FloatPTR(), 0.1f, -180.0f, 180.0f);

			ImGui::Text("Scale: ");
			ImGui::DragFloat3("##Scale", component->scale.FloatPTR(), 0.1f);
			ImGui::TreePop();
		}
	}
}