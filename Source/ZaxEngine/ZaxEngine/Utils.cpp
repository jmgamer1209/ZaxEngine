#include "Utils.h"


string Utils::WString2String(const wstring& ws)
{
    _bstr_t t = ws.c_str();
    char* pchar = (char*)t;
    string result = pchar;
    return result;
}

wstring Utils::String2Wstring(const string& s)
{
    _bstr_t t = s.c_str();
    wchar_t* pwchar = (wchar_t*)t;
    wstring result = pwchar;
    return result;
}

void Utils::Vec3ToArray(glm::vec3& vec3, float array[])
{
    array[0] = vec3[0];
    array[1] = vec3[1];
    array[2] = vec3[2];
}