#include "CameraBinding.h"
#include "mono/metadata/class.h"
#include "Component/Camera.h"
#include "BindingHelper.h"
#include <Core/Debug.h>
#include <Core/InputSystem/Input.h>

namespace ZaxEngine::Binding::Input
{
	bool GetKeyDown(int key)
	{
		return ZaxEngine::InputSystem::Input::GetKeyDown(key);
	}

	bool GetMouseDown(int key)
	{
		return ZaxEngine::InputSystem::Input::GetMouseDown(key);
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.InputSystem.Input::GetKeyDown", reinterpret_cast<void*>(GetKeyDown));
		mono_add_internal_call("ZaxEngine.InputSystem.Input::GetMouseDown", reinterpret_cast<void*>(GetMouseDown));
	}
};
