#pragma once
#include "Platform/windows.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include <iostream>
#include "glad/gl.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

#define GLFW_INIT  glfwSetErrorCallback(glfw_error_callback); \
                                if (!glfwInit()) return 1;

// static void ImGui_ShowDemoWindow() 
// {
// 	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
// }

// EMSCRIPTEN  兼容性代码暂时去掉

//#ifdef __EMSCRIPTEN__
//	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
//	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
//io.IniFilename = nullptr;
//EMSCRIPTEN_MAINLOOP_BEGIN
//#else
//while (!glfwWindowShouldClose(window))
//#endif

//#ifdef __EMSCRIPTEN__
//EMSCRIPTEN_MAINLOOP_END;
//#endif