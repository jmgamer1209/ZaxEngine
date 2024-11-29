#pragma once
#include <string>

#include "Core/Application.h"
#include "Core/Utils.h"
#include "Core/WindowBase.h"
#include "boost/filesystem/path.hpp"
#include "filesystem/operations.hpp"

class OpenWindow: public WindowBase
{
public:
    boost::filesystem::path projectPath;
    bool shouldOpenEditorOnClose = false;
    
public:
    OpenWindow();
    void DrawWindowUI() override;
    void OnWindowClosed() override;
};
