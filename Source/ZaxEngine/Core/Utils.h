#pragma once
#include "Platform/windows.h"
#include <string>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
using namespace std;
#include <glm/glm.hpp>

namespace Utils
{
	string WString2String(const wstring& ws);
	wstring String2Wstring(const string& s);
	void LoadTexture(unsigned int* texture, string& path, bool flip_vertically = false);
	void LoadCubeMap(unsigned int* texture, const string& folderPath);
};

