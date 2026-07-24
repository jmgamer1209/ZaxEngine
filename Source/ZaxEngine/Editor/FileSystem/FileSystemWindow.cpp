#include "FileSystemWindow.h"
#include "Core/Application.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/directory.hpp"
namespace fs = boost::filesystem;
namespace Editor::FileSystem
{
	// 获取指定目录下一级所有子文件夹（不递归深层）
	void FileSystemWindow::FillSubNodes(FileSystemNode& node)
	{	
		boost::filesystem::path dir = this->projectPath / node.relativePath;
		
		if (!boost::filesystem::exists(dir) || !boost::filesystem::is_directory(dir))
		{
			return;
		}
		
		for (const auto& entry : boost::filesystem::directory_iterator(dir))
		{
			
			if (fs::is_directory(entry.path()))
			{
				node.subNodes.push_back(std::make_shared<FileSystemNode>(NodeType::Folder, node.relativePath / entry.path().filename()));
			}
		}
	}

	FileSystemWindow::FileSystemWindow()
	{
		projectPath = Application::projectFolderPath;
		this->rootNode = std::make_unique<FileSystemNode>();
		this->rootNode->type = NodeType::Folder;
		this->rootNode->relativePath = "";
		this->rootNode->isExpanded = false;
	}

	void FileSystemWindow::OnGUI()
	{
		
	}
	FileSystemNode::FileSystemNode()
	{
		this->type = NodeType::Folder;
		this->isExpanded = false;
	}
	FileSystemNode::FileSystemNode(NodeType type, boost::filesystem::path& relativePath)
	{
		this->type = type;
		this->relativePath = relativePath;
		this->isExpanded = false;
	}
}

