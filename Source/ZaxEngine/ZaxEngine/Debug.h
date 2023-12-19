#pragma once

#include <Windows.h>
#include <string>
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
#include <initializer_list>

 namespace Debug
{
	 void Log(const std::string& log);
	 void Log(std::initializer_list<std::string> list);
};

