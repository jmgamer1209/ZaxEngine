#include <windows.h>
#include "ZaxEngine.h"
#include "ShaderProgram.h"
#include "Debug.h"
#include "Utils.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AssetModel.h"
#include "MeshRenderer.h"
#include "DefualtMaterial.h"

void ShowUI();
void UpdateCursorPos();
void UpdateWindowSize();

MeshRenderer* box;
string contentPath;
unsigned int texture;

int viewportWidth;
int viewportHeight;

bool firstMouseRecord = true;
float lastXPos;
float lastYPos;
float xOffsetPos;
float yOffsetPos;

Camera camera;

float yaw, pitch, roll;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	GLFW_INIT;
	
	if (ImGui_Init() == 1) return 1;

	// 设置内容路径
	TCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, path);
	auto projectPath = Utils::WString2String(path);
	contentPath = projectPath + "/../../../Content/";

	// 导入模型
	AssetModel model = AssetModel(contentPath + "Common/WoodenCrate/Wooden Crate.obj");

	// 创建 Shader Program 和 材质
	auto shaderProgram = new ShaderProgram(contentPath + "v0.7/vertex.vs", contentPath + "v0.7/fragment.fs");
	DefaultMaterial mat;
	mat.shader = shaderProgram;
	
	// 创建渲染物体
	box = new MeshRenderer(&model, &(model.meshes[0]), &mat);

	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 以单字节去读取像素，而不是4字节

	camera.position[2] = 10;

   

	while (!glfwWindowShouldClose(window))
	{
		UpdateWindowSize();
		UpdateCursorPos();

		camera.OnViewportChange(viewportWidth, viewportHeight);
//		camera.OnCursorPosChange(xOffsetPos, yOffsetPos);
		camera.HandleCameraInput(window);

		glViewport(0, 0, viewportWidth, viewportHeight);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 渲染物体
		box->Draw(camera);

		// 渲染 UI 界面
		ImGui_NewFrame(); // ImGui 开始绘制
		ShowUI();
		ImGui::Render(); // ImGui 生成渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGui 执行绘制指令

		// 缓冲区交换，将缓冲区数据显示到窗口
		glfwSwapBuffers(window);
	}

	delete box;
	shaderProgram->Delete();

	CleanUp();

	return 0;
}

void ShowUI()
{
	ImGui::Begin("Scene");
		
	//if (ImGui::TreeNode("Cube"))
	//{
	//	ImGui::TreePop();
	//}

	ImGui::Text("CubePosition: "); //ImGui::SameLine();
	ImGui::SliderFloat3("##Position", box->position, -5, 5);

	ImGui::Text("CubeRotation: "); //ImGui::SameLine();
	ImGui::SliderFloat3("##Rotation", box->rotation, 0, 90);
	//ImGui::Text("CubeRotationAngle: "); //ImGui::SameLine();
	//ImGui::SliderFloat("##RotationAngle", &box->rotationAngle, -180, 180);

	ImGui::Text("CubeScale: "); //ImGui::SameLine();
	ImGui::SliderFloat3("##Scale", box->scale, 0, 10);

	ImGui::End();

	ImGui_ShowSimpleWindow();
	ImGui_ShowDemoWindow();
	//ImGui_ShowAnotherWindow();

	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	//ImGui::SameLine();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouseRecord) 
//	{
//		firstMouseRecord = false;
//		lastXPos = xpos;
//		lastYPos = ypos;
//		return;
//	}
//
//	xOffset = xpos - lastXPos;
//	yOffset = ypos - lastYPos;
//
//	lastXPos = xpos;
//	lastYPos = ypos;
//
//	camera.OnCursorPosChange(xOffset, yOffset);
//}

void UpdateWindowSize()
{
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
	camera.viewportWidth = viewportWidth;
	camera.viewportHeight = viewportHeight;
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