#pragma once

#include <Windows.h>
#include <string>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
using namespace std;

namespace Utils
{
	string WString2String(const wstring& ws);
	wstring String2Wstring(const string& s);
};

