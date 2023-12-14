#include "ZaxEngine.h"
#include "ShaderProgram.h"
#include "Debug.h"
#include <Windows.h>
#include "Utils.h"
#include "stb_image.h"

void HandleRenderData();
void DoRender();
void ShowUI();

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;
int success;
char infoLog[512];
float position[3] = { 0, 0, 0 };
ShaderProgram* shaderProgram;
string contentPath; 
unsigned int texture;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	GLFW_INIT; 

	if (ImGui_Init() == 1) return 1;
	
	// 设置内容路径
	TCHAR path[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, path);
	auto projectPath = Utils::WString2String(path);
	contentPath = projectPath + "/../../../Content/";

	// 创建 Shader Program
	shaderProgram = new ShaderProgram(contentPath + "v0.4/vertex.vs", contentPath + "v0.4/fragment.fs");

	// 准备三角形数据
	HandleRenderData();

	while (!glfwWindowShouldClose(window))
	{
		// 设置视口与背景
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// 渲染三角形
		DoRender();

		// 渲染 UI 界面
		ImGui_NewFrame(); // ImGui 开始绘制
		ShowUI();
		ImGui::Render(); // ImGui 生成渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGui 执行绘制指令

		// 缓冲区交换，将缓冲区数据显示到窗口
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shaderProgram->Delete();

	CleanUp();

	return 0;
}

void HandleRenderData()
{
	//*********
	// 准备三角形mesh数据，并将顶点属性配置及顶点绑定放在VAO中
	//*********
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // 左下 
			0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // 右上
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	unsigned int indeces[] = {
		0,1,2,
		0,2,3
	};
	

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	//设置顶点属性的顺序
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//设置颜色布局
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//设置纹理坐标布局
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// 将绑定操作清除，这个不是什么必须的，基本不需要
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindVertexArray(0);

	// 加载纹理
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置环绕方式，下面表示 s 和 t 都设置为 Repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// 设置过滤方式，下面表示 放大缩小时都是线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis
	int width, height, nrChannels;
	unsigned char* data = stbi_load((contentPath + "Common/wall.jpg").c_str(), &width, &height, &nrChannels, 0);
	
	if (data) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	stbi_image_free(data);
}

void DoRender()
{
	shaderProgram->Use();
	shaderProgram->SetUniform("offset", position[0], position[1], position[2]);
	
	// 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, texture);
	shaderProgram->SetUniform("texture1", 0);
	 
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ShowUI()
{
	ImGui::Begin("Triangle");
	
	ImGui::Text("Position: "); //ImGui::SameLine();
	ImGui::SliderFloat3("##Position", position, -1, 1);

	ImGui::End();

	//ImGui_ShowSimpleWindow();
	//ImGui_ShowDemoWindow();
	//ImGui_ShowAnotherWindow();

	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	//ImGui::SameLine();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}