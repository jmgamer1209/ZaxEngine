#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "boost/filesystem/path.hpp"
#include "Window/EditorWindow.h"

namespace ZaxEngine::Editor::FileSystem
{
    class FileImportWindow:public EditorWindow
    {
    public:
        FileImportWindow();
        void OnGUI() override;
        //void SetShow();

    public:
        boost::filesystem::path fileSourcePath;
        boost::filesystem::path targetPath;
    };
}