#include "ScriptObject.h"
#include "./CS/Bindings/BindingHelper.h"
#include <vector>

namespace ZaxEngine::Scripting
{
	void ScriptObject::operator =(MonoObject* obj) 
	{
		this->monoObj = obj;
		this->monoClass = mono_object_get_class(obj);
	}

	bool ScriptObject::IsValidate()
	{
		return monoObj != nullptr;
	}

	std::vector<ScriptField> ScriptObject::GetInspectorFields()
	{
		auto fields = Binding::BindingHelper::GetAllFields(monoObj);
		return fields;
	}

	const char* ScriptObject::GetName()
	{
		return Binding::BindingHelper::GetTypeName(monoObj);
	}
}