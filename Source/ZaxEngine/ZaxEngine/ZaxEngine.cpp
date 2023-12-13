﻿
#include "ZaxEngine.h"

void HandleRenderData();
void DoRender();
void ShowUI();

unsigned int VAO;
unsigned int shaderProgram;
unsigned int VBO;
unsigned int EBO;
int success;
char infoLog[512];
float color[4] = {1,1,1,1};

int wWinMain()
{
	GLFW_INIT;

	if (ImGui_Init() == 1) return 1;
	
	// 准备三角形数据，以及着色器程序，VAO
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
	glDeleteProgram(shaderProgram);

	CleanUp();

	return 0;
}

//*********
// 编译 shader，并生成 shader program
//*********
void PrepareShader()
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 color;\n"
		"void main()\n"
		"{\n"
		"   FragColor = color;\n"
		"}\n\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void HandleRenderData()
{
	PrepareShader();

	//*********
	// 准备三角形mesh数据，并将顶点属性配置及顶点绑定放在VAO中
	//*********
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
	};
	unsigned int indeces[] = {
		0,1,2
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 将绑定操作清除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindVertexArray(0);
}

void DoRender()
{
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ShowUI()
{
	ImGui::Begin("Triangle");

	ImGui::Text("Color: "); //ImGui::SameLine();
	ImGui::SliderFloat4("", color, 0, 1);

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