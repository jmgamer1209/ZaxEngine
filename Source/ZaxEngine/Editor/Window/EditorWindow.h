#pragma once
#include "Core/WindowBase.h"
#include "Renderer/SceneRenderer.h"

class EditorWindow: public WindowBase
{
public:
    SceneRenderer* sceneRenderer;
    GameObject* selectedGO;
    bool isShowLightingSettings;
    Scene* scene;
    //ShaderProgram* shaderProgram;

    EditorWindow();
    void DrawWindowUI() override;
    void LoadScene();
    void DrawScene();
    void PreDrawImgui() override;
    void OnWindowClosed() override;
};
