#include "CameraBinding.h"
#include "mono/metadata/class.h"
#include "Component/Camera.h"
#include "BindingHelper.h"
#include <Core/Debug.h>

namespace ZaxEngine::Binding::Camera
{
	void Create(MonoObject* obj)
	{
		auto native = new ::Camera();
		BindingHelper::SetNativePtr(obj, native);
		native->scriptObj = obj;
		Debug::Log("Create Camera Success");
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.Camera::Internal_Create", reinterpret_cast<void*>(Create));
	}
};
