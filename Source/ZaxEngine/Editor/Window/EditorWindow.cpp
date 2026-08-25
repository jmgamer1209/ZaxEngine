#include "EditorWindow.h"
#include "Core/Application.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/directory.hpp"
#include <vector>
#include <unordered_set>
#include "FileSystem/FileImportWindow.h"
#include "FileSystem/FileSystemWindow.h"

namespace fs = boost::filesystem;
namespace ZaxEngine::Editor
{
	int EditorWindow::gwindowID = 0;
	std::unordered_map<int, std::shared_ptr<EditorWindow>> EditorWindow::showWindowMap;
	std::vector<int> closedWindowList;

	EditorWindow::EditorWindow()
	{
		gwindowID++;
		windowID = gwindowID;
	}

	std::shared_ptr<EditorWindow> EditorWindow::GetWindow(WindowNameType type)
	{
		if (type == WindowNameType::FileImport)
		{
			std::shared_ptr<EditorWindow> window = std::make_shared<FileSystem::FileImportWindow>();
			return window;
		}
		else if (type == WindowNameType::FileSystem){
			std::shared_ptr<EditorWindow> window = std::make_shared<FileSystem::FileSystemWindow>();
			return window;
		}
		return nullptr;
	}

	void EditorWindow::AllOnGUI()
	{
		for (auto& window : showWindowMap) {
			window.second->Internal_OnGUI();
		}
		for (auto& ID : closedWindowList)
		{
			showWindowMap.erase(ID);
		}
		closedWindowList.clear();
	}

	void EditorWindow::Internal_OnGUI()
	{
		if (bShow) {
			std::string name = windowTitle + "##EditorWindow" + std::to_string(windowID);
			if (this->showType == WindowShowType::Normal)
			{
				if (ImGui::Begin(name.c_str(), &bShow))
				{
					OnGUI();
					ImGui::End();
				}
			}
			else
			{
				ImGui::OpenPopup(name.c_str());
				if (ImGui::BeginPopupModal(name.c_str(), &bShow))
				{
					OnGUI();
					ImGui::EndPopup();
				}
			}
			if (!bShow)
			{
				closedWindowList.push_back(windowID);
			}
		}
	}

	void EditorWindow::Close()
	{
		EditorWindow::showWindowMap.erase(windowID);
	}

	void EditorWindow::Show(std::shared_ptr<EditorWindow> window)
	{

		if (EditorWindow::showWindowMap.count(window->windowID) > 0)
		{
			return;
		}
		window->bShow = true;
		EditorWindow::showWindowMap[window->windowID] = window;
	}
}

