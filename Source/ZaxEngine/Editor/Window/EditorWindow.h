#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "boost/filesystem/path.hpp"
#include <string>
#include <unordered_map>
#include "imgui/imgui.h"

namespace ZaxEngine::Editor
{
    enum class WindowNameType {
        FileSystem,
        FileImport
    };

    enum class WindowShowType {
        Normal,
        Popup
    };

    class EditorWindow
    {
    public:
        EditorWindow();
        void virtual OnGUI() = 0;
        
        static void Show(std::shared_ptr<EditorWindow>);
        static std::shared_ptr<EditorWindow> GetWindow(WindowNameType type);

        std::string windowTitle;
        WindowShowType showType = WindowShowType::Normal;
        ImVec2 windowSize;

        static void AllOnGUI();

    private:
        int windowID;
        static int gwindowID;

        static std::unordered_map<int, std::shared_ptr<EditorWindow>> showWindowMap;

        bool bShow = false;
        void Internal_OnGUI();
        void Close();
    };
}