#pragma once
#ifdef APIENTRY
#undef APIENTRY
#endif
#define byte win_byte_override  //  override byte to prevent clashes with <cstddef>
#include <Windows.h>

//  Undefine byte macros so it won't collide with <cstddef> header content.
//  因为 windows 的 byte 和 std 的byte 有冲突
//  注意：只有当项目不使用 windows 库的 byte 类型，才可以这样做，
#undef byte 
