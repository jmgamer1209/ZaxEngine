#include "CameraBinding.h"
#include "mono/metadata/class.h"
#include "Component/Camera.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::Input
{
	bool GetKeyDown(int key)
	{
		Debug::Log(key);
		return false;
	}

	bool GetMouseDown(int key)
	{
		Debug::Log(key);
		return false;
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.InputSystem.Input::GetKeyDown", reinterpret_cast<void*>(GetKeyDown));
		mono_add_internal_call("ZaxEngine.InputSystem.Input::GetMouseDown", reinterpret_cast<void*>(GetMouseDown));
	}
};
