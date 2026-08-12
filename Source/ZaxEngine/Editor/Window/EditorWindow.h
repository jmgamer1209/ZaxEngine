#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "boost/filesystem/path.hpp"

namespace ZaxEngine::Editor
{
    class EditorWindow
    {
    public:
        EditorWindow();
        void virtual OnGUI() = 0;
    };
}