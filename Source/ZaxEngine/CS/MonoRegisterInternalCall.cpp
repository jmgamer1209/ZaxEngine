#include "MonoRegisterInternalCall.h"
#include "Bindings/SceneBinding.h"
#include "Bindings/MeshBinding.h"
#include "Bindings/ShaderProgramBinding.h"

void MonoRegister()
{
	ZaxEngine::Binding::Scene::RegisterInternalCall();
	ZaxEngine::Binding::Mesh::RegisterInternalCall();
	ZaxEngine::Binding::ShaderProgram::RegisterInternalCall();
}