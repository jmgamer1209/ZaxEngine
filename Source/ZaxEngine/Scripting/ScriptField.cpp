#include "ScriptField.h"
#include "CS/Bindings/BindingHelper.h"

namespace ZaxEngine::Scripting 
{
	ScriptField::ScriptField(MonoClassField* field)
	{
		this->field = field;
	}

	ScriptField::ScriptField(MonoClassField* field, MonoObject* obj)
	{
		this->field = field;
		this->obj = obj;
	}

	void ScriptField::operator =(MonoClassField* field) {
		this->field = field;
	}

	bool ScriptField::IsValidate() {
		if (this->field == nullptr) return false;
		return true;
	}

	const char* ScriptField::GetTypeName()
	{
		auto stype = mono_field_get_type(field);
		const char* name = mono_type_get_name(stype);
		return name;
	}

	const char* ScriptField::GetName()
	{
		const char* name = mono_field_get_name(this->field);
		return name;
	}

	int ScriptField::GetInteger()
	{
		return Binding::BindingHelper::MonoObjectGetValue<int>(this->obj, this->field);
	}

	void ScriptField::SetInteger(int value)
	{
		return Binding::BindingHelper::MonoObjectSetValue(obj, field, &value);
	}
}