#include "SkyboxEditor.h"
#include <imgui.h>
#include <Component/Skybox.h>

namespace ZaxEngine::Editor
{
	SkyboxEditor::SkyboxEditor(Component* target): InspectorEditor(target)
	{
	}

	void SkyboxEditor::OnGui()
	{
		auto component = (Skybox*)target;
		if (ImGui::TreeNode("Skybox"))
		{
			/*ImGui::Text("Position: ");
			ImGui::DragFloat3("##Position", component->position.FloatPTR(), 0.1f);

			ImGui::Text("Rotation: ");
			ImGui::DragFloat3("##Rotation", component->rotation.FloatPTR(), 0.1f, -180.0f, 180.0f);

			ImGui::Text("Scale: ");
			ImGui::DragFloat3("##Scale", component->scale.FloatPTR(), 0.1f);*/
			ImGui::TreePop();
		}
	}
}