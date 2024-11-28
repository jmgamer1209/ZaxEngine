#include "OpenWindow.h"

#include <iostream>
#include <glad/gl.h>

#include "Core/Application.h"
#include "Core/Debug.h"
#include "Materials/BlinnPhongMaterial.h"

bool show_demo_window = false; 
char Path[MAX_PATH] = "";

OpenWindow::OpenWindow():WindowBase()
{
    // 设置内容路径
    //TCHAR path[MAX_PATH] = { 0 };
    //GetCurrentDirectory(MAX_PATH, path);
    //auto projectPath = Utils::WString2String(path);
    //printf("%s\n", projectPath.c_str());
    
    
    projectPath = Utils::GetDefaultProjectPath().append("\\Project\\Default");
    printf("%s\n", projectPath.c_str());
    Application::contentPath = projectPath + "\\Content\\";
    strcpy_s(Path,projectPath.c_str());
}

void OpenWindow::DrawWindowUI()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
        return;
    }
    
    // 组件属性检视面板
    ImGui::Begin("Choose");

    ImGui::Text("ProjectPath:");
    if (ImGui::InputText("##ProjectPath", Path,IM_ARRAYSIZE(Path)))
    {
        projectPath = std::string(Path);
        Application::contentPath = projectPath + "\\Content\\";
    }
    ImGui::SameLine();
    if (ImGui::Button("Choose Project"))
    {
        //m_fileDialog.Open();
    }
    
    if (ImGui::Button("Open Project"))
    {
        // 检测 project Path 是不是项目目录
        //auto filePath = 
        if (projectPath.empty() == false)
        {
            glfwSetWindowShouldClose(window, true);
            shouldOpenEditorOnClose = true;    
        }
    }
    ImGui::End();
}

void OpenWindow::OnWindowClosed()
{
    WindowBase::OnWindowClosed();
    if (shouldOpenEditorOnClose)
    {
        Application::OpenEditor();
    }
    else
    {
        Application::isRunning = false;
    }
}
