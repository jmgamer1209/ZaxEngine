#include "OpenWindow.h"

#include <iostream>
#include <glad/gl.h>

#include "Core/Application.h"
#include "Core/Debug.h"
#include "Materials/BlinnPhongMaterial.h"

#include "filesystem/path.hpp"
#include "filesystem/directory.hpp"
#include "filesystem/operations.hpp"
#include <string>

#include "Core/Utils.h"

using namespace boost;

#include <Editor/Window/CommonFileDialogApp.h>


char Path[MAX_PATH] = "";
boost::filesystem::path projectPath;
bool shouldOpenEditorOnClose = false;

OpenWindow::OpenWindow():WindowBase()
{
    filesystem::path exePath(boost::filesystem::initial_path<boost::filesystem::path>());
    auto projectRoot = exePath.parent_path().parent_path();
    projectPath = projectRoot / "Project" / "Default" / "Default.zproject";
    cout << "Default Project Path: " <<  projectPath.string() << "\n";
    strcpy_s(Path,projectPath.string().c_str());
}

bool show_demo_window = false;
bool isProject = false;
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
        cout << projectPath.string() << "\n";
    }
    ImGui::SameLine();
    if (ImGui::Button("Choose Project"))
    {
        string path;
        if (OpenFolderDialog(path))
        {
            projectPath = path;
            strcpy_s(Path,projectPath.string().c_str());
            cout << projectPath.string() << "\n";
        }
    }
    
    if (ImGui::Button("Open Project"))
    {
        // 检测 project Path 是不是项目目录
        isProject = false;
        auto  extension = projectPath.extension();
        if (extension == ".zproject")
        {
            isProject = true;
        }
        if (isProject)
        {
            glfwSetWindowShouldClose(window, true);
            shouldOpenEditorOnClose = true;    
        }
        else
        {
            ImGui::OpenPopup("Error##popup_no_project");
        }
    }

    // 如果不是项目，则提醒
    if (ImGui::BeginPopupModal("Error##popup_no_project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::NewLine();
        ImGui::Text("The File is Not Zax Project!");
        ImGui::Separator();
        ImGui::NewLine();
        
        auto size = ImGui::GetWindowSize();
        auto btnSize = ImVec2(80, 0);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX()+size.x/2-btnSize.x/2);
        if (ImGui::Button("OK", btnSize)) { ImGui::CloseCurrentPopup(); }
        //ImGui::SetItemDefaultFocus();
        // ImGui::SameLine();
        // if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }    

    ImGui::End();
}

void OpenWindow::OnWindowClosed()
{
    WindowBase::OnWindowClosed();
    if (shouldOpenEditorOnClose)
    {
        Application::OpenEditor(projectPath);
    }
    else
    {
        Application::isRunning = false;
    }
}
