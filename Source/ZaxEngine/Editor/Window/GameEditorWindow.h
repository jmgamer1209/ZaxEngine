#pragma once
#include "Core/WindowBase.h"
#include "Renderer/SceneRenderer.h"
#include "FileSystem/FileSystemWindow.h"

class GameEditorWindow: public ZaxEngine::Platform::WindowBase
{
public:
    SceneRenderer* sceneRenderer;
    GameObject* selectedGO;
    bool isShowLightingSettings;
    Scene* scene;
    std::shared_ptr<ZaxEngine::Editor::FileSystem::FileSystemWindow> fileSystemWindow;
    //ShaderProgram* shaderProgram;

    GameEditorWindow();
    void DrawWindowUI() override;
    void LoadScene();
    void DrawScene();
    void PreDrawImgui() override;
    void OnWindowClosed() override;
    

private:
    void HandleDeltaTime();
    void GameLogicUpdate();
    void GameLogicStart();
private:
    int sceneViewWidth = 1280;
    int sceneViewHeight = 720;
};
