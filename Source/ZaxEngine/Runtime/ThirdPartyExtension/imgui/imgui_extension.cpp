#include "imgui_extension.h"

namespace ImGui
{
	void LayoutBottomCenterOnWindow(ImVec2 size, float marginToBottom)
	{
		ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
		ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
		ImVec2 contentArea = ImVec2(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

		// 设置光标到底部居中
		ImGui::SetCursorPos(ImVec2(
			contentMin.x + (contentArea.x - size.x) * 0.5f,
			contentMax.y - size.y - marginToBottom
		));
	}
}