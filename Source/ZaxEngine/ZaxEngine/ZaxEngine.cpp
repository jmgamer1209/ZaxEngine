
#include "ZaxEngine.h"

int wWinMain()
{
	GLFW_INIT;

	if (ImGui_Init() == 1) return 1;

	while (!glfwWindowShouldClose(window))
	{
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// ImGui 开始绘制
		ImGui_NewFrame();

		// ImGui 显示窗口
		ImGui_ShowSimpleWindow();
		ImGui_ShowDemoWindow();
		ImGui_ShowAnotherWindow();

		// ImGui 生成渲染数据
		ImGui::Render();

		// ImGui 执行绘制指令
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 缓冲区交换，将缓冲区数据显示到窗口
		glfwSwapBuffers(window);
	}

	CleanUp();

	return 0;
}
