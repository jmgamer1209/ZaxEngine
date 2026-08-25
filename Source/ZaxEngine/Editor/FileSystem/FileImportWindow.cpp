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
	}
}

