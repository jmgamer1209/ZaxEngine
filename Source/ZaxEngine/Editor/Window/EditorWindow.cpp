#include "EditorWindow.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "boost/json.hpp"
#include "boost/filesystem/path.hpp"

#include "Core/SceneManager.h"
#include "CS/MonoEntry.h"
#include "CS/MonoRegisterInternalCall.h"


#include "Core/PlatformTime.h"

using namespace boost;

EditorWindow::EditorWindow():WindowBase()
{
	auto config = Utils::LoadJsonFile(Application::projectFolderPath / "Config" / "DefaultEngine.json");
	auto value = config["EditorStartupMap"].as_string();
	Application::projectConfig.EditorStartupMap = value.c_str();
	Debug::Log(value.c_str());
	try {
		MonoEntry::GetInstance()->RunGameStart();
	}
	catch (const std::exception& e) {
		Debug::Log(e.what());
	}
	
}

void EditorWindow::OnWindowClosed()
{
	delete scene;
	//shaderProgram->Delete();
	WindowBase::OnWindowClosed();
	Application::isRunning = false;
}

void EditorWindow::DrawScene()
{
	if (sceneRenderer == nullptr)
	{
		sceneRenderer = new SceneRenderer();
		sceneRenderer->Init(viewportWidth, viewportHeight);    
	}
	if (!isInMinimal) sceneRenderer->Draw(scene);
}

void EditorWindow::PreDrawImgui()
{
	HandleDeltaTime();
	if (scene == nullptr)
	{
		LoadScene();
	}
	GameLogicUpdate();
	DrawScene();
}

void EditorWindow::GameLogicUpdate()
{
	for (size_t i = 0; i < this->scene->list.size(); i++)
	{
		auto go = this->scene->list[i];
		for (size_t j = 0; j < go->components.size(); j++) 
		{
			auto comp = go->components[j];
			if (comp->updateFunc.IsValidate())
			{
				comp->updateFunc.Call();
			}
		}
	}
}

void EditorWindow::HandleDeltaTime()
{
	//声明 lastRealTime 并同时初始化
	static double lastRealTime = PlatformTime::Seconds() - 0.0001;
	double CurrentRealTime = PlatformTime::Seconds();
	double RealDeltaTime = CurrentRealTime - lastRealTime;
	lastRealTime = CurrentRealTime;
	Debug::Log(RealDeltaTime);
}


void EditorWindow::DrawWindowUI()
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

void EditorWindow::LoadScene()
{
	scene = SceneManager::mainScene; //用脚本端创建的场景

	/*******

	// 导入 Mesh
	Mesh* woodenBox = new Mesh(Application::contentPath / "Common" / "WoodenCrate" / "Wooden Crate.obj");

	// 创建 Shader Program 和 材质
	shaderProgram = new ShaderProgram(Application::contentPath / "Shaders" / "Common" / "forward.vs", Application::contentPath / "Shaders" / "Common" / "forward.fs");
	Material* mat = new Material(shaderProgram);

	// 加载纹理
	std::string baseColorPath = (Application::contentPath / "Common" / "WoodenCrate" / "Textures" / "Wooden Crate_Crate_BaseColor.png").string();
	std::string normalPath = (Application::contentPath / "Common" / "WoodenCrate" / "Textures" / "Wooden Crate_Crate_Normal.png").string();
	auto woodenBoxAlbedoTexture = Texture::Load(baseColorPath);
	auto woodenBoxNormalTexture = Texture::Load(normalPath);

	mat->SetProperty("AlbedoTexture", MaterialProperty(woodenBoxAlbedoTexture));
	mat->SetProperty("NormalMap", MaterialProperty(woodenBoxNormalTexture));

	Material* planeMat = new Material(shaderProgram);
	planeMat->SetProperty("AlbedoTexture", MaterialProperty(woodenBoxAlbedoTexture));


	Material* transparentMat = new Material(shaderProgram);
	transparentMat->SetProperty("AlbedoTexture", MaterialProperty(woodenBoxAlbedoTexture));
	transparentMat->SetProperty("NormalMap", MaterialProperty(woodenBoxNormalTexture));
	transparentMat->SetProperty("SurfaceType", MaterialProperty(1));
	transparentMat->SetProperty("Alpha", MaterialProperty(0.5f));


	// 先创建天空盒
	auto skyboxGO = new GameObject("Skybox");
	skyboxGO->AddComponent(new Transform());
	auto skybox = new Skybox();
	auto folderPath = (Application::contentPath / "/Common/Skybox/1").string();
	auto cubeMapTex = Texture::Load(folderPath, TextureType::CubeMap);
	skybox->SetCubeMap(cubeMapTex);
	skyboxGO->AddComponent(skybox);

	// 反射材质
	auto reflectionShader = new ShaderProgram(Application::contentPath / "Shaders" / "Common" / "reflectionCube.vs", Application::contentPath / "Shaders" / "Common" / "reflectionCube.fs");
	auto reflectionMat = new Material(reflectionShader);
	auto albedoTexture = woodenBoxAlbedoTexture;
	reflectionMat->SetProperty("albedoTexture", MaterialProperty(albedoTexture));
	reflectionMat->SetProperty("environmentTexture", MaterialProperty(skybox->GetCubeMap()));
	reflectionMat->SetProperty("specularIntensity", MaterialProperty(0.5f));
	reflectionMat->SetProperty("reflectionIntensity", MaterialProperty(0.5f));

	// 创建渲染物体
	vector<GameObject*> boxes;
	for (size_t i = 1; i <= 4; i++)
	{
		auto box = new GameObject(string("Box")+std::to_string(i));
		boxes.push_back(box);
		box->AddComponent(new Transform());
		if (i == 3) box->AddComponent(new MeshRenderer(woodenBox, transparentMat));
		else if (i < 4) box->AddComponent(new MeshRenderer(woodenBox, mat));
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

	// 创建1个平面
	auto planeGO = new GameObject(string("Plane"));
	auto transform = new Transform();
	transform->position = Vector3(0, -10, -5);
	transform->rotation = Vector3(-90, 0, 0);
	transform->scale = Vector3(15);
	planeGO->AddComponent(transform);
	planeGO->AddComponent(new MeshRenderer(Mesh::GetQuadMesh(), planeMat));
	

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
	light->range = 60;
	transform->position = Vector3(-2.0f, 10.0f, 3.0f); 
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
	spotLightGO->AddComponent(light);

	// 创建场景
	scene = SceneManager::mainScene; //用脚本端创建的场景
	/*scene->lightingSettings.ambientColor = Color(1, 1, 1);
	scene->lightingSettings.ambientIntensity = 0.1f;*/

	/**
	scene->AddGameObject(cameraGO);
	for (size_t i = 0; i < boxes.size(); i++)
	{
		scene->AddGameObject(boxes[i]);
	}
	scene->AddGameObject(lightGO);
	scene->AddGameObject(pointLightGO);
	scene->AddGameObject(spotLightGO);
	scene->AddGameObject(skyboxGO);
	scene->AddGameObject(planeGO);
	*******/
}