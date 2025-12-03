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

	bool ScriptField::TypeNameEqual(const char* target)
	{
		return strcmp(this->GetTypeName(), target) == 0;
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

	bool ScriptField::GetBool()
	{
		return Binding::BindingHelper::MonoObjectGetValue<bool>(this->obj, this->field);
	}

	void ScriptField::SetBool(bool value)
	{
		return Binding::BindingHelper::MonoObjectSetValue(obj, field, &value);
	}

	float ScriptField::GetFloat()
	{
		return Binding::BindingHelper::MonoObjectGetValue<float>(this->obj, this->field);
	}

	void ScriptField::SetFloat(float value)
	{
		return Binding::BindingHelper::MonoObjectSetValue(obj, field, &value);
	}

	void ScriptField::SetString(const char* value)
	{
		auto monostr = mono_string_from_utf16((mono_unichar2*)ZString(value).GetU16String().c_str());
		return Binding::BindingHelper::MonoObjectSetValue(this->obj, this->field, monostr);
	}

	std::string ScriptField::GetString()
	{
		auto monostr = Binding::BindingHelper::MonoObjectGetValue<MonoString*>(obj, field);
		return Binding::BindingHelper::StringFromMonoString(monostr);
	}
}