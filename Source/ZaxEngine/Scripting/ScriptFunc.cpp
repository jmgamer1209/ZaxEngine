#include "ScriptFunc.h"
#include "CS/Bindings/BindingHelper.h"

namespace ZaxEngine::Scripting 
{
	void ScriptFunc::operator =(MonoMethod* method) {
		this->method = method;
	}

	bool ScriptFunc::IsValidate() {
		if (method == nullptr) return false;
		return true;
	}

	void ScriptFunc::Call()
	{
		Binding::BindingHelper::MonoInvoke(method, obj, NULL);
	}
}