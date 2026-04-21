#include "DebugBinding.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include "Core/Debug.h"
#include "BindingHelper.h"
#include "BindingCommon.h"
#include "imgui.h"
#include "Core/Vector.h"

namespace ZaxEngine::Binding::ImGui {

	bool Internal_Button(MonoString* label)
	{
		auto str = BindingHelper::StringFromMonoString(label);
		return ::ImGui::Button(str.c_str());
	}
	
	bool Internal_ButtonWithSize(MonoString* label, Vector2 size)
	{
		auto str = BindingHelper::StringFromMonoString(label);
		return ::ImGui::Button(str.c_str(), ImVec2(size.x, size.y));
	}

	void Internal_SetCursorPos(float x, float y)
	{
		return ::ImGui::SetCursorPos(ImVec2(x, y));
	}

	void RegisterInternalCall() 
	{
		mono_add_internal_call("ZaxEngine.ImGui::Button", reinterpret_cast<void*>(Internal_Button));
		mono_add_internal_call("ZaxEngine.ImGui::Button", reinterpret_cast<void*>(Internal_ButtonWithSize));
        mono_add_internal_call("ZaxEngine.ImGui::SetCursorPos", reinterpret_cast<void*>(Internal_SetCursorPos));
	}
}
