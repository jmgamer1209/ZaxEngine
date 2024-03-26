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
#include "Renderer/SceneRenderer.h"
#include "Renderer/FrameBuffer.h"
#include "Core/Application.h"
#include "Component/Skybox.h"
#include "Materials/ReflectionCubeMaterial.h"

void LoadScene();
void DrawScene();
void ShowUI();
void UpdateCursorPos();
void UpdateWindowSize();

bool firstMouseRecord = true;
float lastXPos;
float lastYPos;
float xOffsetPos;
float yOffsetPos;

Scene* scene;
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
	Application::contentPath = projectPath + "/Content/";

	Application::viewportWidth = viewportWidth;
	Application::viewportHeight = viewportHeight;

	LoadScene();

	sceneRenderer = new SceneRenderer();
	sceneRenderer->Init(viewportWidth, viewportHeight);

	while (!glfwWindowShouldClose(window))
	{
		UpdateWindowSize();
		UpdateCursorPos();

		glViewport(0, 0, viewportWidth, viewportHeight);

		if (!Application::isInMinimal) DrawScene();

		// 渲染 UI 界面
		ImGui_NewFrame(); // ImGui 开始绘制
		if (!Application::isInMinimal) ShowUI();
		ImGui::Render(); // ImGui 生成渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGui 执行绘制指令
		
		// 缓冲区交换，将缓冲区数据显示到窗口
		glfwSwapBuffers(window); 

		Application::isViewportSizeChanged = false;
	}

	delete scene;
	shaderProgram->Delete();

	CleanUp();

	return 0;
}

void LoadScene()
{
	// 导入模型
	AssetModel* model = new AssetModel(Application::contentPath + "Common/WoodenCrate/Wooden Crate.obj");
	Mesh* woodenBox = model->meshes[0].CreateMesh();
	auto texturePath = model->materials[(model->meshes[0].materialIndex)].baseColor.path;

	// 创建 Shader Program 和 材质
	shaderProgram = new ShaderProgram(Application::contentPath + "Shaders/Common/forward.vs", Application::contentPath + "Shaders/Common/forward.fs");
	BlinnPhongMaterial* mat = new BlinnPhongMaterial(shaderProgram, texturePath);
	BlinnPhongMaterial* planeMat = new BlinnPhongMaterial(shaderProgram, texturePath);

	// 先创建天空盒
	auto skyboxGO = new GameObject("Skybox");
	skyboxGO->AddComponent(new Transform());
	auto skybox = new Skybox();
	skyboxGO->AddComponent(skybox);

	// 反射材质
	auto reflectionShader = new ShaderProgram(Application::contentPath + "Shaders/Common/reflectionCube.vs", Application::contentPath + "Shaders/Common/reflectionCube.fs");
	auto reflectionMat = new ReflectionCubeMaterial(reflectionShader, model, &(model->meshes[0]), skybox->GetCubeMap());

	// 创建渲染物体
	vector<GameObject*> boxes;
	for (size_t i = 1; i <= 4; i++)
	{
		auto box = new GameObject(string("Box")+std::to_string(i));
		boxes.push_back(box);
		box->AddComponent(new Transform());
		if (i < 4) box->AddComponent(new MeshRenderer(woodenBox, mat));
		else box->AddComponent(new MeshRenderer(woodenBox, reflectionMat));

		Vector3 position(0, 0, 0);
		Vector3 rotation(0, 0, 0);

		if (i == 1) {
			position = Vector3(-11.0f, -2.5f, -8.0f);
			rotation = Vector3(45.0f, -30.0f, 0);
		}
		else if (i == 2){
			position = Vector3(-2.5f, -2.5f, 0);
			rotation = Vector3(45.0f, 0, 0);
		}
		else if (i == 3)
		{
			position = Vector3(6.5f, -2.5f, 0);
			rotation = Vector3(45.0f, 0, -30.0f);
		}
		else if (i == 4)
		{
			position = Vector3(4.5f, 2.0f, -2.0f);
			rotation = Vector3(0, 35.0f, 0);
		}
		box->GetComponent<Transform>()->position = position;
		box->GetComponent<Transform>()->rotation = rotation;
	}

	auto planeGO = new GameObject(string("Plane"));
	auto transform = new Transform();
	transform->position = Vector3(0, -10, -5);
	transform->rotation = Vector3(-90, 0, 0);
	transform->scale = Vector3(15);
	planeGO->AddComponent(transform);
	planeGO->AddComponent(new MeshRenderer(Mesh::GetQuadMesh(), mat));
	

	// 设置摄像机
	auto cameraGO = new GameObject("Camera");
	cameraGO->AddComponent(new Transform());
	auto camera = new Camera();
	cameraGO->AddComponent(camera);
	cameraGO->GetComponent<Transform>()->position.z = 30;
	auto post = new PostProcess();
	post->enabled = false;
	cameraGO->AddComponent(post);

	// 创建光源
	Light* light;
	auto lightGO = new GameObject("DirectionalLight");
	transform = new Transform();
	lightGO->AddComponent(transform);
	light = new Light(LightType::Directional);
	lightGO->AddComponent(light);
	transform->position = Vector3(0,10,0);
	transform->rotation = Vector3(-90, 0, 0);

	auto pointLightGO = new GameObject("PointLight");
	transform = new Transform();
	pointLightGO->AddComponent(transform);
	light = new Light(LightType::Point);
	light->color = { 1,0,0 };
	transform->position = Vector3(-2.0f, 1.5f, 3.0f); 
	pointLightGO->AddComponent(light);

	auto spotLightGO = new GameObject("SpotLight");
	transform = new Transform();
	transform->position = Vector3(7, 20, 0);
	transform->rotation = Vector3(-90, 0, 0);
	spotLightGO->AddComponent(transform);
	light = new Light(LightType::Spot);
	light->color = { 0,1,0 };
	light->range = 100;
	light->innerAngle = 30;
	light->outerAngle = 40;
	light->shadowMapSize = 2048;
	spotLightGO->AddComponent(light);

	// 创建场景
	scene = new Scene();
	scene->lightingSettings.ambientColor = Color(1, 1, 1);
	scene->lightingSettings.ambientIntensity = 0.1f;
	scene->AddGameObject(cameraGO);
	for (size_t i = 0; i < boxes.size(); i++)
	{
		scene->AddGameObject(boxes[i]);
	}
	scene->AddGameObject(lightGO);
//	scene->AddGameObject(pointLightGO);
	scene->AddGameObject(spotLightGO);
	scene->AddGameObject(skyboxGO);
	scene->AddGameObject(planeGO);
}

void DrawScene()
{
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

	if (ImGui::BeginMenu("View Mode"))
	{
		static int viewMode = (int)sceneRenderer->viewMode;
		if (ImGui::Combo("##View Mode", &viewMode, "Lit\0Depth\0\0")) {
			sceneRenderer->viewMode = (ViewMode)viewMode;
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
		ImGui::Text("isActive: "); ImGui::SameLine();
		ImGui::Checkbox("##isActive", &selectedGO->isActive);

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
		ImGui::ColorEdit3("##Ambient Color", scene->lightingSettings.ambientColor.FloatPTR());
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
	int preWidth = viewportWidth;
	int preHeight = viewportHeight;
	
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
	Application::viewportWidth = viewportWidth;
	Application::viewportHeight = viewportHeight;
	Application::isInMinimal = viewportWidth == 0 && viewportHeight == 0;
	Application::isViewportSizeChanged = preWidth != viewportWidth || preHeight != viewportHeight;
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