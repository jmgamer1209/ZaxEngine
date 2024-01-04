#include "ZaxEngine.h"
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
#include "Core/SceneRenderer.h"

void LoadScene();
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
ShaderProgram* shaderProgram;
SceneRenderer* sceneRenderer;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	GLFW_INIT;
	
	if (ImGui_Init() == 1) return 1;

	// 设置内容路径
	TCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, path);
	auto projectPath = Utils::WString2String(path);
	contentPath = projectPath + "/Content/";

	LoadScene();

	sceneRenderer = new SceneRenderer();

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

void LoadScene()
{
	// 导入模型
	AssetModel* model = new AssetModel(contentPath + "Common/WoodenCrate/Wooden Crate.obj");

	// 创建 Shader Program 和 材质
	shaderProgram = new ShaderProgram(contentPath + "v0.10/vertex.vs", contentPath + "v0.10/fragment.fs");
	BlinnPhongMaterial* mat = new BlinnPhongMaterial(shaderProgram);

	// 创建渲染物体
	auto box = new GameObject("Box");
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
	Light* light;
	auto lightGO = new GameObject("DirectionalLight");
	lightGO->AddComponent(new Transform());
	light = new Light(LightType::Directional);
	lightGO->AddComponent(light);

	auto pointLightGO = new GameObject("PointLight");
	auto transform = new Transform();
	pointLightGO->AddComponent(transform);
	light = new Light(LightType::Point);
	light->color[0] = 1;
	light->color[1] = 0;
	light->color[2] = 0;
	transform->position[1] = 1.5f;
	transform->position[2] = 3.0f;
	pointLightGO->AddComponent(light);

	auto spotLightGO = new GameObject("SpotLight");
	transform = new Transform();
	transform->position[1] = -2;
	transform->position[2] = 10;
	spotLightGO->AddComponent(transform);
	light = new Light(LightType::Spot);
	light->color[0] = 0;
	light->color[1] = 1;
	light->color[2] = 0;
	light->range = 20;
	light->innerAngle = 8;
	light->outerAngle = 10;
	spotLightGO->AddComponent(light);

	// 创建场景
	scene = new Scene();
	scene->lightingSettings.ambientColor[0] = 1;
	scene->lightingSettings.ambientColor[1] = 1;
	scene->lightingSettings.ambientColor[2] = 1;
	scene->lightingSettings.ambientIntensity = 0.1f;
	scene->AddGameObject(cameraGO);
	scene->AddGameObject(box);
	scene->AddGameObject(lightGO);
	scene->AddGameObject(pointLightGO);
	scene->AddGameObject(spotLightGO);
}

void DrawScene()
{
	// 摄像机
	camera->OnViewportChange(viewportWidth, viewportHeight);
	//camera->HandleCameraInput(window);

	sceneRenderer->Draw(scene);
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
		ImGui::Text(selectedGO->name.c_str());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

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
		ImGui::Text("Ambient Color:");
		ImGui::ColorEdit3("##Ambient Color", scene->lightingSettings.ambientColor);
		ImGui::Text("Ambient Intensity:");
		ImGui::DragFloat("##Ambient Intensity", &scene->lightingSettings.ambientIntensity, 0.01f, 0, 1);

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