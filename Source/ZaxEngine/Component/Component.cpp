#pragma once
#include "Component.h"
#include "CS/Bindings/BindingHelper.h"
using namespace ZaxEngine::Binding;

void Component::OnGui()
{
	BindingHelper::GetAllFields(scriptObj.monoObj);
}
