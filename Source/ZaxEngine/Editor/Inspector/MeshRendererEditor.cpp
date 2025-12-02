#include "MeshRendererEditor.h"
#include <imgui.h>
#include <Component/MeshRenderer.h>

namespace ZaxEngine::Editor
{
	MeshRendererEditor::MeshRendererEditor(Component* target): InspectorEditor(target)
	{
	}

	void MeshRendererEditor::OnGui()
	{
		auto component = (MeshRenderer*)target;
		if (ImGui::TreeNode("MeshRenderer"))
		{
			component->mat->OnGui();

			ImGui::TreePop();
		}
	}
}