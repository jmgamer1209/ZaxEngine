#pragma once
#include <string>
#include <wchar.h>
using namespace std;

/// <summary>
/// 以 utf-16 编码的字符串
/// 采用 utf-16 的理由：利于C#端与C++端交互，并且对于 Windows 端系统库也比较友好，还有不少库也是支持 utf-16
/// </summary>
class ZString
{
protected:
	u16string data;
public:
	ZString();
	ZString(const wchar_t* wstr);
	ZString(const uint16_t* utf16_str);
	string GetU8String();
};

