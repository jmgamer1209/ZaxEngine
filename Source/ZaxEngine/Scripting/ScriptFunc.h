#pragma once
#include "mono/metadata/object.h"
#include "Core/Debug.h"

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
	};
}


