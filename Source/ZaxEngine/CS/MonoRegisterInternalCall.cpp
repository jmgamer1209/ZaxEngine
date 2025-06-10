#include "MonoRegisterInternalCall.h"
#include "Bindings/SceneBinding.h"
#include "Bindings/MeshBinding.h"
#include "Bindings/ShaderProgramBinding.h"
#include "Bindings/MaterialBinding.h"
#include "Bindings/TextureBinding.h"

void MonoRegister()
{
	ZaxEngine::Binding::Scene::RegisterInternalCall();
	ZaxEngine::Binding::Mesh::RegisterInternalCall();
	ZaxEngine::Binding::ShaderProgram::RegisterInternalCall();
	ZaxEngine::Binding::Material::RegisterInternalCall();
	ZaxEngine::Binding::Texture::RegisterInternalCall();
}