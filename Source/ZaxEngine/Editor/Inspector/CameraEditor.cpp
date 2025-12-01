#include "CameraEditor.h"
#include <imgui.h>
#include <Component/Camera.h>

namespace ZaxEngine::Editor
{
	CameraEditor::CameraEditor(Component* target): InspectorEditor(target)
	{
	}

	void CameraEditor::OnGui()
	{
		auto camera = (Camera*)target;
		if (ImGui::TreeNode("Camera")) {
			ImGui::Text("Fov-Vertical:");
			ImGui::DragFloat("##Fov-Vertical", &(camera->fovy), 1.0f, 0.1f, 179.0f);
			ImGui::Text("Near:");
			ImGui::DragFloat("##Near", &(camera->clippingNear), 1.0f, 0, (float)INT32_MAX);
			ImGui::Text("Far:");
			ImGui::DragFloat("##Far", &(camera->clippingFar), 1.0f, 0, (float)INT32_MAX);

			ImGui::TreePop();
		}
	}
}