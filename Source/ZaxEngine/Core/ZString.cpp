#include "ZString.h"
#include <cassert>
#include <vector>
#include "ThirdParty/boost/locale/encoding.hpp"

ZString::ZString()
{
}

ZString::ZString(const wchar_t* wstr) 
{
    if (!wstr) data = std::u16string(); // 处理空指针

    // Windows 平台（wchar_t 为 16 位）
#ifdef _WIN32
    static_assert(sizeof(wchar_t) == sizeof(char16_t),"wchar_t size mismatch on Windows");
    if (wstr != nullptr) {
        data = std::u16string(reinterpret_cast<const char16_t*>(wstr));
    }
#else
    // Linux/macOS 平台（wchar_t 为 32 位）
    static_assert(sizeof(wchar_t) == 4,"wchar_t size mismatch on Unix-like systems");

    try {
        std::wstring wstr_32(wstr);
        data = boost::locale::conv::utf_to_utf<char16_t>(wstr_32);
    }
    catch (...) {
        data = std::u16string(); // 转换失败
    }
#endif
}
ZString::ZString(const uint16_t* utf16_str) 
{
    if (utf16_str != nullptr) {
        data = std::u16string(reinterpret_cast<const char16_t*>(utf16_str));
    }
}

string ZString::GetU8String() 
{
    return boost::locale::conv::utf_to_utf<char>(data);
}
