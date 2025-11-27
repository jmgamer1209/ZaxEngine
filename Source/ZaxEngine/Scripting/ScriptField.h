#pragma once
#include "mono/metadata/object.h"
#include "Core/Debug.h"

namespace ZaxEngine::Scripting
{
	/// <summary>
	/// 脚本字段
	/// </summary>
	struct ScriptField
	{
		MonoObject* obj = nullptr;
		MonoClassField* field = nullptr;
		const char* typeName = nullptr;
		const char* fieldName = nullptr;

		ScriptField(MonoClassField* field);
		ScriptField(MonoClassField* field, MonoObject* obj);

		void operator =(MonoClassField* field);

		bool IsValidate();

		const char* GetTypeName();
		const char* GetName();
		int GetInteger();
		void SetInteger(int);
	};
}


