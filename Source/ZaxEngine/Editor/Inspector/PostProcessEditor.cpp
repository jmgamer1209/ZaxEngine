#include "PostProcessEditor.h"
#include <imgui.h>
#include <Component/PostProcess.h>

namespace ZaxEngine::Editor
{
	PostProcessEditor::PostProcessEditor(Component* target): InspectorEditor(target)
	{
	}

	void PostProcessEditor::OnGui()
	{
		auto component = (PostProcess*)target;
		if (ImGui::TreeNode("PostProcess"))
		{
			ImGui::Text("Enable: "); ImGui::SameLine();
			ImGui::Checkbox("##Enable:", &component->enabled);
			ImGui::TreePop();
		}
	}
}