#pragma once
#include "mono/metadata/object.h"

namespace ZaxEngine::Scripting
{
	/// <summary>
	/// 脚本对象，根据不同的脚本封装不同类型，比如使用C#脚本，则对象为 Mono，如果是 Lua，则应该是 Lua
	/// </summary>
	struct ScriptFunc
	{
		MonoObject* obj = nullptr;
		MonoMethod* method = nullptr;

		void operator =(MonoMethod* method) {
			this->method = method;
		}

		bool IsValidate() {
			if (method == nullptr) return false;
			return true;
		}

		void Call() 
		{
			mono_runtime_invoke(method, obj, NULL, NULL);
		}
	};
}


