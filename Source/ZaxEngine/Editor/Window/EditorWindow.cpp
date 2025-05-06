#include "EditorWindow.h"

#include <fstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <sstream>

#include "Materials/BlinnPhongMaterial.h"
#include "Materials/ReflectionCubeMaterial.h"
#include "boost/json.hpp"
#include "boost/filesystem/path.hpp"
#include "Core/SceneManager.h"
#include "WindowsMain/MonoEntry.h"

using namespace boost;

EditorWindow::EditorWindow():WindowBase()
{
	auto config = Utils::LoadJsonFile(Application::projectFolderPath / "Config" / "DefaultEngine.json");
	auto value = config["EditorStartupMap"].as_string();
	Application::projectConfig.EditorStartupMap = value.c_str();
	Debug::Log(value.c_str());

	MonoEntry::GetInstance()->LoadAssembly(Application::projectFolderPath / "bin" / "Debug" / "net8.0" / Application::projectName.replace_extension(".dll"));
}

void EditorWindow::OnWindowClosed()
{
	delete scene;
	shaderProgram->Delete();
	WindowBase::OnWindowClosed();
	Application::isRunning = false;
}

void EditorWindow::DrawScene()
{
	if (scene == nullptr)
	{
		LoadScene();
	}
	if (sceneRenderer == nullptr)
	{
		sceneRenderer = new SceneRenderer();
		sceneRenderer->Init(viewportWidth, viewportHeight);    
	}
	if (!isInMinimal) sceneRenderer->Draw(scene);
}

void EditorWindow::PreDrawImgui()
{
	DrawScene();
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
	//SceneManager::LoadScene(Application::contentPath / Application::projectConfig.EditorStartupMap);

	
	// 导入模型
	AssetModel* model = new AssetModel(Application::contentPath / "Common" / "WoodenCrate" / "Wooden Crate.obj");
	Mesh* woodenBox = model->meshes[0].CreateMesh();
	AssetMaterial*  woodenBoxMat = &model->materials[model->meshes[0].materialIndex];

	// 创建 Shader Program 和 材质
	shaderProgram = new ShaderProgram(Application::contentPath / "Shaders" / "Common" / "forward.vs", Application::contentPath / "Shaders" / "Common" / "forward.fs");
	BlinnPhongMaterial* mat = new BlinnPhongMaterial(shaderProgram, &model->materials[model->meshes[0].materialIndex]);
	BlinnPhongMaterial* planeMat = new BlinnPhongMaterial(shaderProgram, woodenBoxMat->baseColor.path);
	BlinnPhongMaterial* transparentMat = new BlinnPhongMaterial(shaderProgram, &model->materials[model->meshes[0].materialIndex]);
	transparentMat->SetProperty("SurfaceType", MaterialProperty(1));
	transparentMat->SetProperty("Alpha", MaterialProperty(0.5f));


	// 先创建天空盒
	auto skyboxGO = new GameObject("Skybox");
	skyboxGO->AddComponent(new Transform());
	auto skybox = new Skybox();
	skyboxGO->AddComponent(skybox);

	// 反射材质
	auto reflectionShader = new ShaderProgram(Application::contentPath / "Shaders" / "Common" / "reflectionCube.vs", Application::contentPath / "Shaders" / "Common" / "reflectionCube.fs");
	auto reflectionMat = new ReflectionCubeMaterial(reflectionShader, model, &(model->meshes[0]), skybox->GetCubeMap());

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
	scene = new Scene();
	scene->lightingSettings.ambientColor = Color(1, 1, 1);
	scene->lightingSettings.ambientIntensity = 0.1f;
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
}