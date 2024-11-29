#include "OpenWindow.h"

#include <iostream>
#include <glad/gl.h>

#include "Core/Application.h"
#include "Core/Debug.h"
#include "Materials/BlinnPhongMaterial.h"

using namespace boost;

bool show_demo_window = false; 
char Path[MAX_PATH] = "";


OpenWindow::OpenWindow():WindowBase()
{
    filesystem::path exePath(boost::filesystem::initial_path<boost::filesystem::path>());
    auto projectRoot = exePath.parent_path().parent_path();
    projectPath = projectRoot / ("Project") / ("Default");
    cout << "Default Project Path: " <<  projectPath.string() << "\n";

    Application::contentPath = (projectPath / ("Content")).string();
    strcpy_s(Path,projectPath.string().c_str());
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
        Application::contentPath = (projectPath / "Content").string();
        cout << Application::contentPath << "\n";
    }
    ImGui::SameLine();
    if (ImGui::Button("Choose Project"))
    {
        //m_fileDialog.Open();
    }
    
    if (ImGui::Button("Open Project"))
    {
        // 检测 project Path 是不是项目目录
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
