#include "PostProcessBinding.h"
#include "mono/metadata/class.h"
#include "Component/PostProcess.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::PostProcess
{
	void Create(MonoObject* obj)
	{
		auto native = new ::PostProcess();
		BindingHelper::SetNativePtr(obj, native);
		native->scriptObj = obj;
		Debug::Log("Create PostProcess Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.PostProcess::Internal_Create", reinterpret_cast<void*>(Create));
	}
};
