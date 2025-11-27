#include "BindingHelper.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "CS/MonoEntry.h"
#include "Component/Component.h"
#include "Component/Transform.h"
#include "BindingCommon.h"
#include "Scripting/ScriptField.h"
using namespace ZaxEngine::Scripting;

namespace ZaxEngine::Binding::BindingHelper {

	void SetNativePtr(MonoObject* obj, void* value)
	{
		auto mono_class = mono_object_get_class(obj);
		MonoClassField* field = mono_class_get_field_from_name(mono_class, "nativePtr");
		mono_field_set_value(obj, field, &value);
	}
	
	MonoObject* NewMonoObject(const char* name_space, const char* name) {
		MonoClass* mono_class = mono_class_from_name(MonoEntry::GetInstance()->image_engine, name_space, name);
		return mono_object_new(MonoEntry::GetInstance()->domain, mono_class);
	}

	void MonoObjectSetValue(MonoObject* obj, const char* fieldName, void* value) {
		auto mono_class = mono_object_get_class(obj);
		auto mono_field = mono_class_get_field_from_name(mono_class, fieldName);
		mono_field_set_value(obj, mono_field, value);
	}

	void MonoObjectSetValue(MonoObject* obj, MonoClassField* field, void* value)
	{
		mono_field_set_value(obj, field, value);
	}

	string StringFromMonoString(MonoString* str)
	{
		return ZString(mono_string_to_utf16(str)).GetU8String();
	}

	::Component* GetComponent(GameObject* go, const char* name_space, const char* class_name) 
	{
		if (name_space != "ZaxEngine") return nullptr;
		if (class_name == "Transform")
		{
			return go->GetComponent<::Transform>();
		}
		return nullptr;
	}

	bool IsSubOfGameComponent(MonoObject* obj)
	{
		auto objClass = mono_object_get_class(obj);
		auto componentClass = mono_class_from_name(MonoEntry::GetInstance()->image_engine, BindingCommon::EngineNameSpace, "GameComponent");
		return mono_class_is_subclass_of(objClass, componentClass, false);
	}

	MonoMethod* GetUpdateFunc(MonoObject* obj) {
		auto objClass = mono_object_get_class(obj);
		auto updateMethod = mono_class_get_method_from_name(objClass, "Update", 0);
		return updateMethod;
	}

	MonoObject* MonoInvoke(MonoMethod* method, void* obj, void** params)
	{
		MonoObject* exc = NULL;
		MonoObject* result = mono_runtime_invoke(method, obj, params, &exc);
		if (exc) {
			MonoString* exc_msg = mono_object_to_string(exc, NULL);
			Debug::Log(mono_string_to_utf8(exc_msg));
		}
		return result;
	}

	const char* GetTypeName(MonoObject* obj)
	{
		auto objClass = mono_object_get_class(obj);
		auto objClassName = mono_class_get_name(objClass);
		return objClassName;
	}

	std::vector<ScriptField> GetAllFields(MonoObject* obj)
	{
		void* iter = NULL;
		MonoClassField* field;
		
		std::vector<ScriptField> list = {};
		auto objClass = mono_object_get_class(obj);

		while ((field = mono_class_get_fields(objClass, &iter))) {
			const char* name = mono_field_get_name(field);
			MonoType* type = mono_field_get_type(field);
			const char* type_name = mono_type_get_name(type);
			ScriptField sfield(field, obj);
			list.push_back(sfield);
		}
		return list;
	}
}
