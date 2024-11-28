#pragma once
#include <string>

#include "Core/Application.h"
#include "Core/Utils.h"
#include "Core/WindowBase.h"

class OpenWindow: public WindowBase
{
public:
    std::string projectPath = "";
    bool shouldOpenEditorOnClose = false;
    
public:
    OpenWindow();
    void DrawWindowUI() override;
    void OnWindowClosed() override;
};
