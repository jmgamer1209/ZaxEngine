#include "FileImportWindow.h"
#include "Core/Application.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/directory.hpp"
#include <vector>
#include <unordered_set>

namespace fs = boost::filesystem;
namespace ZaxEngine::Editor::FileSystem
{
	FileImportWindow::FileImportWindow()
	{
		windowTitle = "资源导入设置";
		this->showType = WindowShowType::Popup;
	}
	void FileImportWindow::OnGUI()
	{
		ImGui::Text("测试资源");
		ImVec2 buttonSize = ImVec2(120, 30);
		float margin = 10.0f;

		ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
		ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
		ImVec2 contentArea = ImVec2(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

		// 设置光标到底部居中
		ImGui::SetCursorPos(ImVec2(
			contentMin.x + (contentArea.x - buttonSize.x) * 0.5f,
			contentMax.y - buttonSize.y - margin
		));

		if (ImGui::Button("确定", buttonSize))
		{

		}
	}
}

