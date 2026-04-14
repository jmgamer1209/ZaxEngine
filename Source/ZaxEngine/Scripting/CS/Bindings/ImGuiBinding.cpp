#include "DebugBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "imgui.h"

namespace ZaxEngine::Binding::ImGui {

	bool Internal_Button(MonoString* label)
	{
		auto str = BindingHelper::StringFromMonoString(label);
		return ::ImGui::Button(str.c_str());
	}

	void RegisterInternalCall() 
	{
		mono_add_internal_call("ZaxEngine.ImGui::Button", reinterpret_cast<void*>(Internal_Button));
	}
}
