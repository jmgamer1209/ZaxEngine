#pragma once
#include "mono/metadata/object.h"

namespace ZaxEngine::Scripting
{
	struct ScriptObject
	{
		MonoClass* monoClass;

		void operator =(MonoObject* obj) {
			this->monoClass = mono_object_get_class(obj);
		}
	};
}


