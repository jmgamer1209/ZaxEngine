#pragma once

#include <Windows.h>
#include <string>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
using namespace std;
#include <glm/glm.hpp>

namespace Utils
{
	string WString2String(const wstring& ws);
	wstring String2Wstring(const string& s);
	void Vec3ToArray(glm::vec3& vec3, float array[]);
	void LoadTexture(unsigned int* texture, string& path, bool flip_vertically = false);
};

