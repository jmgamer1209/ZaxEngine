﻿#include "ZaxEngine.h"
#include "Core/ShaderProgram.h"
#include "Core/Debug.h"
#include "Core/Utils.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Component/Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Assets/AssetModel.h"
#include "Component/MeshRenderer.h"
#include "Materials/BlinnPhongMaterial.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Component/Light.h"
#include "Component/Transform.h"

void DrawScene();
void ShowUI();
void UpdateCursorPos();
void UpdateWindowSize();

string contentPath;

int viewportWidth;
int viewportHeight;
bool isInMinimal;

bool firstMouseRecord = true;
float lastXPos;
float lastYPos;
float xOffsetPos;
float yOffsetPos;

Scene* scene;
Camera* camera;
Light* light;
float ambient;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	GLFW_INIT;
	
	if (ImGui_Init() == 1) return 1;

	// 设置内容路径
	TCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, path);
	auto projectPath = Utils::WString2String(path);
	contentPath = projectPath + "/Content/";

	// 导入模型
	AssetModel* model = new AssetModel(contentPath + "Common/WoodenCrate/Wooden Crate.obj");

	// 创建 Shader Program 和 材质
	ShaderProgram* shaderProgram = new ShaderProgram(contentPath + "v0.9/vertex.vs", contentPath + "v0.9/fragment.fs");
	BlinnPhongMaterial* mat = new BlinnPhongMaterial(shaderProgram);
	
	// 创建渲染物体
	auto box  = new GameObject("Box");
	box->AddComponent(new Transform());
	box->AddComponent(new MeshRenderer(model, &(model->meshes[0]), mat));
	box->GetComponent<Transform>()->position[1] = -2.5f;	// 因为 box 的中心点在底部，所以微调一下
	box->GetComponent<Transform>()->rotation[0] = 45.0f;

	// 设置摄像机
	auto cameraGO = new GameObject("Camera");
	cameraGO->AddComponent(new Transform());
	camera = new Camera();
	cameraGO->AddComponent(camera);
	cameraGO->GetComponent<Transform>()->position[2] = 20;

	// 创建光源
	auto lightGO = new GameObject("Light");
	lightGO->AddComponent(new Transform());
	light = new Light();
	lightGO->AddComponent(light);

	// 创建场景
	scene = new Scene();
	scene->AddGameObject(cameraGO);
	scene->AddGameObject(box);
	scene->AddGameObject(lightGO);

	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 以单字节去读取像素，而不是4字节

	while (!glfwWindowShouldClose(window))
	{
		UpdateWindowSize();
		UpdateCursorPos();

		glViewport(0, 0, viewportWidth, viewportHeight);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!isInMinimal) DrawScene();

		// 渲染 UI 界面
		ImGui_NewFrame(); // ImGui 开始绘制
		if (!isInMinimal) ShowUI();
		ImGui::Render(); // ImGui 生成渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGui 执行绘制指令
		
		// 缓冲区交换，将缓冲区数据显示到窗口
		glfwSwapBuffers(window); 
	}

	delete scene;
	shaderProgram->Delete();

	CleanUp();

	return 0;
}

void DrawScene()
{
	// 摄像机
	camera->OnViewportChange(viewportWidth, viewportHeight);
	//camera->HandleCameraInput(window);

	// 渲染物体
	for (size_t i = 0; i < scene->list.size(); i++)
	{
		auto renderer = scene->list[i]->GetComponent<MeshRenderer>();
		if (renderer == nullptr) continue;

		renderer->Draw(camera, light, ambient);
	}
}

GameObject* selectedGO;
bool isShowLightingSettings;
void ShowUI()
{
	// 菜单设置
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Scene Settings"))
	{
		if (ImGui::MenuItem("Lighting")) 
		{
			isShowLightingSettings = true;
		}
		ImGui::EndMenu();
	}
	

	ImGui::EndMainMenuBar();

	// 场景物体
	ImGui::Begin("Scene");
		
	for (size_t i = 0; i < scene->list.size(); i++)
	{
		auto go = scene->list[i];
		if (ImGui::Selectable(go->name.c_str(), false))
		{
			selectedGO = go;
		}
	}

	ImGui::End();

	// 组件属性检视面板
	ImGui::Begin("Inspector");

	if (selectedGO != nullptr)
	{
		for (size_t i = 0; i < selectedGO->components.size(); i++)
		{
			selectedGO->components[i]->OnGui();
		}
	}

	ImGui::End();

	// 光照设置
	if (isShowLightingSettings)
	{
		ImGui::Begin("Lighting Settings", &isShowLightingSettings);

		ImGui::Text("Ambient Intensity:");
		ImGui::DragFloat("##Ambient Intensity", &ambient, 0.01f, 0, 1);

		ImGui::End();
	}

	//ImGui_ShowDemoWindow();
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void UpdateWindowSize()
{
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
	isInMinimal = viewportWidth == 0 && viewportHeight == 0;
}

void UpdateCursorPos()
{
	double tempxPos, tempyPos;
	if (firstMouseRecord)
	{
		firstMouseRecord = false;
		glfwGetCursorPos(window, &tempxPos, &tempyPos);
		lastXPos = (float)tempxPos;
		lastYPos = (float)tempyPos;
	}
	glfwGetCursorPos(window, &tempxPos, &tempyPos);

	xOffsetPos = (float)tempxPos - lastXPos;
	yOffsetPos = (float)tempyPos - lastYPos;

	lastXPos = (float)tempxPos;
	lastYPos = (float)tempyPos;


}