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

bool isViewportSizeChanged;
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

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportWidth, viewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);  // 绑定颜色缓冲

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportWidth, viewportWidth); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);   // 绑定深度和模板缓冲

	// 检查是否完成创建
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Debug::Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	
	// 重新绑定到初始帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float screenVertices[] = {
		-1, -1, 0, 0, 0,
		 1, -1, 0, 1, 0,
		 1,  1, 0, 1, 1,
		-1,-1, 0, 0, 0,
		 1,  1, 0, 1, 1,
		-1, 1, 0, 0, 1
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

	// 创建 Shader Program 和 材质
	auto screenShaderProgram = new ShaderProgram(contentPath + "v0.11/screen.vs", contentPath + "v0.11/screen.fs");

	while (!glfwWindowShouldClose(window))
	{
		UpdateWindowSize();
		UpdateCursorPos();

		glViewport(0, 0, viewportWidth, viewportHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// 如果窗口变化，需要重新设置纹理和rb的大小
		if (isViewportSizeChanged)
		{
			glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportWidth, viewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportWidth, viewportWidth);
		}

		if (!isInMinimal) DrawScene();

		// 将离屏图像绘制到四边形
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		screenShaderProgram->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		screenShaderProgram->SetUniform("screenTex", 0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// 渲染 UI 界面
		ImGui_NewFrame(); // ImGui 开始绘制
		if (!isInMinimal) ShowUI();
		ImGui::Render(); // ImGui 生成渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGui 执行绘制指令
		
		// 缓冲区交换，将缓冲区数据显示到窗口
		glfwSwapBuffers(window); 

		isViewportSizeChanged = false;
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
	BlinnPhongMaterial* mat = new BlinnPhongMaterial(shaderProgram, model, &(model->meshes[0]));

	// 创建渲染物体
	vector<GameObject*> boxes;
	for (size_t i = 1; i <= 3; i++)
	{
		auto box = new GameObject(string("Box")+std::to_string(i));
		boxes.push_back(box);
		box->AddComponent(new Transform());
		box->AddComponent(new MeshRenderer(model, &(model->meshes[0]), mat));

		if (i == 1) {
			box->GetComponent<Transform>()->position = Vector3(-11.0f, -2.5f, -8.0f);
			box->GetComponent<Transform>()->rotation = Vector3(45.0f, -30.0f, 0);
		}
		else if (i == 2){
			box->GetComponent<Transform>()->position = Vector3(-2.5f, -2.5f, 0);
			box->GetComponent<Transform>()->rotation = Vector3(45.0f, 0, 0);
		}
		else if (i == 3)
		{
			box->GetComponent<Transform>()->position = Vector3(6.5f, -2.5f, 0);
			box->GetComponent<Transform>()->rotation = Vector3(45.0f, 0, -30.0f);
		}
	}

	// 设置摄像机
	auto cameraGO = new GameObject("Camera");
	cameraGO->AddComponent(new Transform());
	camera = new Camera();
	cameraGO->AddComponent(camera);
	cameraGO->GetComponent<Transform>()->position.z = 30;

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
	light->color = { 1,0,0 };
	transform->position = Vector3(-2.0f, 1.5f, 3.0f); 
	pointLightGO->AddComponent(light);

	auto spotLightGO = new GameObject("SpotLight");
	transform = new Transform();
	transform->position = Vector3(7, -2, 10);
	spotLightGO->AddComponent(transform);
	light = new Light(LightType::Spot);
	light->color = { 0,1,0 };
	light->range = 20;
	light->innerAngle = 8;
	light->outerAngle = 10;
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
}

void DrawScene()
{
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	isInMinimal = viewportWidth == 0 && viewportHeight == 0;
	isViewportSizeChanged = preWidth != viewportWidth || preHeight != viewportHeight;
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