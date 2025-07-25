#pragma once
#include "mono/metadata/object.h"
#include "Core/Debug.h"

namespace ZaxEngine::Scripting
{
	/// <summary>
	/// 脚本对象，根据不同的脚本封装不同类型，比如使用C#脚本，则对象为 Mono，如果是 Lua，则应该是 Lua
	/// </summary>
	struct ScriptFunc
	{
		MonoObject* obj = nullptr;
		MonoMethod* method = nullptr;

		void operator =(MonoMethod* method);

		bool IsValidate();

		void Call();
	};
}


