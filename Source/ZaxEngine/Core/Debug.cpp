#include "Debug.h"
#include <iostream>

void Debug::Log(const std::string& log)
{
	_bstr_t t = (log+"\n").c_str();
	//wchar_t* pwchar = (wchar_t*)t;
	
	// 控制台和调试器都输出
	std::cout << log << "\n";
	OutputDebugStringA(t);
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
	auto temp = std::to_string(log) + "\n";

	// 控制台和调试器都输出
	std::cout << temp;
	OutputDebugStringA(temp.c_str());
}