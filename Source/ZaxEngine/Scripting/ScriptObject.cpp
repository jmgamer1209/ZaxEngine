#include "ScriptObject.h"

namespace ZaxEngine::Scripting
{
	void ScriptObject::operator =(MonoObject* obj) 
	{
		this->monoObj = obj;
		this->monoClass = mono_object_get_class(obj);
	}
}