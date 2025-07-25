#pragma once

#include"Platform/windows.h"
#include <string>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
#include <initializer_list>

 namespace Debug
{
	 void Log(const std::string& log);
	 void Log(std::initializer_list<std::string> list);

	 void Log(const std::wstring& log);
	 //void Log(std::initializer_list<std::wstring> list);

	 void Log(const int log);
};

