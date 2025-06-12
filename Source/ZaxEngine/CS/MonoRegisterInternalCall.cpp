#include "MonoRegisterInternalCall.h"
#include "Bindings/SceneBinding.h"
#include "Bindings/MeshBinding.h"
#include "Bindings/ShaderProgramBinding.h"
#include "Bindings/MaterialBinding.h"
#include "Bindings/TextureBinding.h"
#include "Bindings/GameObjectBinding.h"
#include "Bindings/TransformBinding.h"
#include "Bindings/SkyboxBinding.h"
#include "Bindings/MeshRendererBinding.h"
#include "Bindings/CameraBinding.h"
#include "Bindings/PostProcessBinding.h"
#include "Bindings/ComponentBinding.h"

void MonoRegister()
{
	ZaxEngine::Binding::Scene::RegisterInternalCall();
	ZaxEngine::Binding::Mesh::RegisterInternalCall();
	ZaxEngine::Binding::ShaderProgram::RegisterInternalCall();
	ZaxEngine::Binding::Material::RegisterInternalCall();
	ZaxEngine::Binding::Texture::RegisterInternalCall();
	ZaxEngine::Binding::GameObject::RegisterInternalCall();
	ZaxEngine::Binding::Transform::RegisterInternalCall();
	ZaxEngine::Binding::Skybox::RegisterInternalCall();
	ZaxEngine::Binding::MeshRenderer::RegisterInternalCall();
	ZaxEngine::Binding::Camera::RegisterInternalCall();
	ZaxEngine::Binding::PostProcess::RegisterInternalCall();
	ZaxEngine::Binding::Component::RegisterInternalCall();
}