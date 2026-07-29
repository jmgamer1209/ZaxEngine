#include "FileSystemWindow.h"
#include "Core/Application.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/directory.hpp"
#include <vector>

namespace fs = boost::filesystem;
namespace ZaxEngine::Editor::FileSystem
{
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
	}

	void FileSystemWindow::OnGUI_ShowNode(FileSystemNode& node)
	{
		if (node.type == NodeType::File)
		{
			ImGui::Text(node.relativePath.filename().string().c_str());
		}
		else
		{
			string name = "";
			if (node.isRoot)
			{
				name = "Root";
			}
			else {
				name = node.relativePath.filename().string();
			}
			FillSubNodes(node);
			//Debug::Log(name);
			if (ImGui::TreeNode(name.c_str()))
			{
				node.isExpanded = true;
				
				for (size_t i = 0; i < node.subNodes.size(); i++)
				{
					OnGUI_ShowNode(*(node.subNodes[i]));
				}
				ImGui::TreePop();
			}
			else
			{
				node.isExpanded = false;
			}
		}
	}

	void FileSystemWindow::OnGUI()
	{
		// 组件属性检视面板
		ImGui::Begin("File System");

		OnGUI_ShowNode(*rootNode);

		ImGui::End();
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

