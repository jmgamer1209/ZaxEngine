#pragma once
#include "mono/metadata/object.h"
#include "ScriptField.h"
#include <vector>

namespace ZaxEngine::Scripting
{
	/// <summary>
	/// 脚本对象，根据不同的脚本封装不同类型，比如使用C#脚本，则对象为 Mono，如果是 Lua，则应该是 Lua
	/// </summary>
	struct ScriptObject
	{
		MonoObject* monoObj;
		MonoClass* monoClass;

		void operator =(MonoObject* obj); 

		bool IsValidate();

		const char* GetName();

		std::vector<ScriptField> GetInspectorFields();
	};
}


