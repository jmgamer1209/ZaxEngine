#include "CameraBinding.h"
#include "mono/metadata/class.h"
#include "Component/Camera.h"
#include "BindingHelper.h"
#include "Core/Debug.h"
#include "Audio/AudioEngine.h"

namespace ZaxEngine::Binding::AudioManager
{
	void Play(MonoString* path)
	{
		std::string pathStr = BindingHelper::StringFromMonoString(path);
		Audio::AudioEngine::GetInstance().PlaySound(pathStr.c_str());
	}

	void RegisterInternalCall()
	{
		mono_add_internal_call("ZaxEngine.AudioManager::Internal_Play", reinterpret_cast<void*>(Play));
	}
};
