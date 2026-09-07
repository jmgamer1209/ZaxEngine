#include "FileImportWindow.h"
#include "Core/Application.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/directory.hpp"
#include <vector>
#include <unordered_set>
#include "ThirdPartyExtension/imgui/imgui_extension.h"

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
		ImGui::LayoutBottomCenterOnWindow(buttonSize, 10);
		if (ImGui::Button("确定", buttonSize))
		{

		}
	}
}

