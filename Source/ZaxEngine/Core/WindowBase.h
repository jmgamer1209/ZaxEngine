#pragma once
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

class WindowBase
{
public:
    GLFWwindow* window;
    bool isClosed = false;
    const char* windowTitle = "ZaxEngine";
    int viewportWidth = 1280;
    int viewportHeight = 720;
    bool isInMinimal;
    bool firstMouseRecord = true;
    float lastXPos;
    float lastYPos;
    float xOffsetPos;
    float yOffsetPos;

    WindowBase()
    {
        ImGui_Init();
    }
    
    void Update();
    int ImGui_Init();
    void LoadFont();
    void ImGui_NewFrame();
    void UpdateWindowSize();
    void UpdateCursorPos();
    void ProcessInput(GLFWwindow* window);

    virtual void DrawWindowUI() {}
    virtual void PreDrawImgui() {}
    virtual void OnWindowClosed()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        isClosed = true;
    };
};
