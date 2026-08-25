#include "FileSystemWindow.h"
#include "Core/Application.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/directory.hpp"
#include <vector>
#include <unordered_set>
#include <Window/CommonFileDialogApp.h>
#include "FileImportWindow.h"

namespace fs = boost::filesystem;
namespace ZaxEngine::Editor::FileSystem
{
	std::unordered_set<std::string> rootFolderSet = { "Content", "Source"};
	std::shared_ptr<FileImportWindow> importWindowPtr;
	static bool isOpenImport = false;
	fs::path importSourcePath;
	fs::path importTargetPath;

	// 获取指定目录下一级所有子文件夹（不递归深层）
	void FileSystemWindow::FillSubNodes(FileSystemNode& node)
	{	
		boost::filesystem::path dir = this->projectPath / node.relativePath;
		
		if (!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir))
		{
			return;
		}
		node.subNodes.clear();
		for (const auto& entry : boost::filesystem::directory_iterator(dir))
		{
			auto nodeType = NodeType::File;
			if (fs::is_directory(entry.path())) nodeType = NodeType::Folder;
			if (node.isRoot)
			{
				if (rootFolderSet.count(entry.path().filename().string()) == false)
				{
					continue;
				}
			}
			auto entryNode = std::make_shared<FileSystemNode>(nodeType, node.relativePath / entry.path().filename());
			node.subNodes.push_back(entryNode);
		}
	}

	FileSystemWindow::FileSystemWindow()
	{
		projectPath = Application::projectFolderPath;
		this->rootNode = std::make_unique<FileSystemNode>();
		this->rootNode->type = NodeType::Folder;
		this->rootNode->relativePath = "";
		this->rootNode->isExpanded = false;
		this->rootNode->isRoot = true;
		windowTitle = "File System";
	}


	string select_item_name = "";
	void FileSystemWindow::OnGUI_ShowNode(FileSystemNode& node)
	{
		if (node.type == NodeType::File)
		{
			float indent = ImGui::GetTreeNodeToLabelSpacing();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
			auto name = node.relativePath.filename().string() + "##" + node.relativePath.string();
			if (ImGui::Selectable(name.c_str(), select_item_name == name))
			{
				select_item_name = name;
			}
		}
		else
		{
			string nodeID = "";
			string name = "";
			if (node.isRoot)
			{
				name = "Root";
				nodeID = "Root";
			}
			else {
				name = node.relativePath.filename().string();
				nodeID = node.relativePath.filename().string() + "##" + node.relativePath.string();
			}
			FillSubNodes(node);
			//Debug::Log(name);
			
			auto isExpanded = ImGui::TreeNodeEx(nodeID.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (select_item_name == nodeID ? ImGuiTreeNodeFlags_Selected : 0));

			// 右键弹出菜单，绑定当前item
			if (ImGui::BeginPopupContextItem(nodeID.c_str()))
			{
				select_item_name = nodeID; // 右键点击时选中该节点
				if (ImGui::MenuItem("资源导入")) { 
					auto fullPath = (Application::projectFolderPath / node.relativePath).string();
					if (OpenFileDialog(fullPath))
					{
						Debug::Log(fullPath);
						importSourcePath = fullPath;
						importTargetPath = Application::projectFolderPath / node.relativePath;
						auto importWindow = EditorWindow::GetWindow(WindowNameType::FileImport);
						EditorWindow::Show(importWindow);
					}
				}
				ImGui::EndPopup();
			}

			// 左键点击选中
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
			{
				select_item_name = nodeID;
			}

			node.isExpanded = isExpanded;
			if (isExpanded)
			{	
				for (size_t i = 0; i < node.subNodes.size(); i++)
				{
					OnGUI_ShowNode(*(node.subNodes[i]));
				}
				ImGui::TreePop();
			}
		}
	}

	void FileSystemWindow::OnGUI()
	{
		OnGUI_ShowNode(*rootNode);
	}
	FileSystemNode::FileSystemNode()
	{
		this->type = NodeType::Folder;
		this->isExpanded = false;
	}
	FileSystemNode::FileSystemNode(NodeType type, const boost::filesystem::path& relativePath)
	{
		this->type = type;
		this->relativePath = relativePath;
		this->isExpanded = false;
	}
}

