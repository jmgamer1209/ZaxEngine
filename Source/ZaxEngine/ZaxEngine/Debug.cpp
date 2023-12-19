#include "Debug.h"

void Debug::Log(const std::string& log)
{
	_bstr_t t = (log+"\n").c_str();
	wchar_t* pwchar = (wchar_t*)t;
	OutputDebugString(pwchar);
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
