#include "Debug.h"
#include <iostream>
#include "Utils.h"

void Debug::Log(const std::string& log)
{
	// 控制台输出
	std::cout << log << "\n";
	// 调试器输出
	// 不使用 OutputDebugStringA，因为带A的函数，参数使用的是 ascii 编码而不是 utf8
	auto temp = log + "\n";
	OutputDebugString(Utils::String2Wstring(temp).c_str());  
}

void Debug::Log(std::initializer_list<std::string> list)
{
	std::string str;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		str += *it;
	}
	Log(str);
}

void Debug::Log(const std::wstring& log)
{
	/*_bstr_t t = (log + "\n").c_str();
	wchar_t* pwchar = (wchar_t*)t;*/

	auto temp = log + L"\n";

	// 控制台和调试器都输出
	std::cout << temp.c_str();
	OutputDebugStringW(temp.c_str());
}

void Debug::Log(const int log)
{
	auto temp = std::to_string(log);
	Debug::Log(temp);
}

void Debug::Log(const double log)
{
	auto temp = std::to_string(log);
	Debug::Log(temp);
}