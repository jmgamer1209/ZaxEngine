#pragma once

#include "Core/WindowBase.h"

class OpenWindow: public WindowBase
{
public:
    
public:
    OpenWindow();
    void DrawWindowUI() override;
    void OnWindowClosed() override;
};
