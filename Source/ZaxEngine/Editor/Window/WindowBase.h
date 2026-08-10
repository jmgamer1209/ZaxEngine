#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "boost/filesystem/path.hpp"

namespace ZaxEngine::Editor
{
    class WindowBase
    {
    public:
        WindowBase();
        void virtual OnGUI() = 0;
    };
}