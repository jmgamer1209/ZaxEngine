#include "MonoRegisterInternalCall.h"
#include "Bindings/SceneBinding.h"

void MonoRegister()
{
	ZaxEngine::Binding::Scene::RegisterInternalCall();
}