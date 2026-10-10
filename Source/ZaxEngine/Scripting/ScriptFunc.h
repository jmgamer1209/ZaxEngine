#pragma once
#include "mono/metadata/object.h"
import Debug;

namespace ZaxEngine::Scripting
{
	/// <summary>
	/// 脚本函数，由 MonoMethod 构成
	/// </summary>
	struct ScriptFunc
	{
		MonoObject* obj = nullptr;
		MonoMethod* method = nullptr;

		void operator =(MonoMethod* method);

		bool IsValidate();

		void Call();
		void Call(void** params);
	};
}


