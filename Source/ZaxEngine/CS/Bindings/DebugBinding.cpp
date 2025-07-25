#include "DebugBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "BindingHelper.h"
#include "BindingCommon.h"

namespace ZaxEngine::Binding::Debug {

	void Internal_Log(MonoString* monoStr)
	{
		auto log = BindingHelper::StringFromMonoString(monoStr);
		::Debug::Log(log);
	}

	void RegisterInternalCall() 
	{
		mono_add_internal_call("ZaxEngine.Debug::Log", reinterpret_cast<void*>(Internal_Log));
	}
}
