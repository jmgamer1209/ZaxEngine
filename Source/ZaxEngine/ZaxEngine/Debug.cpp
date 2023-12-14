#include "Debug.h"

void Debug::Log(const std::string log)
{
	_bstr_t t = log.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	OutputDebugString(pwchar);
}
