#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "boost/filesystem/path.hpp"
#include "Window/EditorWindow.h"

namespace ZaxEngine::Editor::FileSystem
{
	enum class NodeType:std::uint8_t
    {
        File,
        Folder
	};

    struct FileSystemNode
    {
    public:
        NodeType type;       // 类型
        bool     isExpanded; // 是否展开
        boost::filesystem::path relativePath;  // 相对路径
        std::vector<std::shared_ptr<FileSystemNode>> subNodes;  // 子目录
        bool isRoot = false;

    public:
        FileSystemNode();
        FileSystemNode(NodeType type, const boost::filesystem::path& relativePath);
    };

    class FileSystemWindow: public EditorWindow
    {
    public:
        FileSystemWindow();
        void OnGUI() override;
        void OnGUI_ShowNode(FileSystemNode& node);
    private:
        boost::filesystem::path projectPath;
        std::unique_ptr<FileSystemNode> rootNode;

    private:
        void FillSubNodes(FileSystemNode& node);
    };
}