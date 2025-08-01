#include "WindowBase.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glad/gl.h>

#include "Application.h"

void WindowBase::Update()
{
	if (isClosed) return;
	
    if (!glfwWindowShouldClose(window))
    {
        UpdateWindowSize();
        UpdateCursorPos();

        glViewport(0, 0, viewportWidth, viewportHeight);

    	PreDrawImgui();
    	
        // 渲染 UI 界面
        ImGui_NewFrame(); // ImGui 开始绘制
        if (!Application::isInMinimal) DrawWindowUI();
        ImGui::Render(); // ImGui 生成渲染数据
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ImGui 执行绘制指令
		
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

        // 缓冲区交换，将缓冲区数据显示到窗口
        glfwSwapBuffers(window); 

        Application::isViewportSizeChanged = false;
    }
    else
    {
	    OnWindowClosed();
    }
}

int WindowBase::ImGui_Init()
{
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 330 core";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// 为了在初始的时候，可以设置窗口的位置，根据 glfwCreateWindow 的注释，需要先隐藏，设置完 Pos 后，再显示
	// 如果不这样做，创建窗口后再调整位置，会出现启动跳变的情况。
	glfwWindowHint(GLFW_VISIBLE, 0);

	// 重置标题栏，这是因为 imgui 开启了 multi viewport 后，在 hub 界面在关闭时，如果有子界面是窗口，则导致 glfw 会遗留错误配置
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); 

	// 创建窗口
	window = glfwCreateWindow(viewportWidth, viewportHeight, windowTitle, nullptr, nullptr);
	if (window == nullptr)
		return 1;

	// 位置设置完后，需要再显示
	GLFWmonitor* pMonitor = glfwGetPrimaryMonitor(); //GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
	const GLFWvidmode* mode = glfwGetVideoMode(pMonitor);

	glfwSetWindowPos(window, (mode->width - viewportWidth) / 2, (mode->height - viewportHeight) / 2);
	glfwShowWindow(window);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	 //初始化 glad
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << "\n";
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	io.IniFilename = nullptr;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	LoadFont();

	return 0;
}

void WindowBase::LoadFont()
{
	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);
}

void WindowBase::ImGui_NewFrame()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void WindowBase::UpdateWindowSize()
{
    int preWidth = viewportWidth;
    int preHeight = viewportHeight;
	
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    Application::viewportWidth = viewportWidth;
    Application::viewportHeight = viewportHeight;
    Application::isInMinimal = viewportWidth == 0 && viewportHeight == 0;
    Application::isViewportSizeChanged = preWidth != viewportWidth || preHeight != viewportHeight;
}

void WindowBase::UpdateCursorPos()
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

void WindowBase::ProcessInput(GLFWwindow* window)
{
	// if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	// 	glfwSetWindowShouldClose(window, true);
}