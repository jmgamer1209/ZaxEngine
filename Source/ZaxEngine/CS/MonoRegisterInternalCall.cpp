#include "MonoRegisterInternalCall.h"
#include "Bindings/SceneBinding.h"
#include "Bindings/MeshBinding.h"

void MonoRegister()
{
	ZaxEngine::Binding::Scene::RegisterInternalCall();
	ZaxEngine::Binding::Mesh::RegisterInternalCall();
}