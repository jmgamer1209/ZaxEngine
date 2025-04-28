#pragma once
#include "Platform/windows.h"
#include <string>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
#include <glm/glm.hpp>
#include <Component/Transform.h>
#include "boost/json.hpp"
#include "filesystem/path.hpp"

namespace Utils
{
	char* WChar2Char(const wchar_t* wchar);
	string WString2String(const wstring& ws);
	wstring String2Wstring(const string& s);
	void LoadTexture(unsigned int* texture, string& path, bool flip_vertically = false);
	void LoadCubeMap(unsigned int* texture, const string& folderPath);
	glm::mat4 GetViewMatrix(const Transform& transform);
	boost::filesystem::path GetExeDirectory();
	std::string GetDefaultProjectPath();
	boost::json::object LoadJsonFile(const boost::filesystem::path& path);
	
};

