#pragma once
#include "Component.h"
#include "CS/Bindings/BindingHelper.h"
using namespace ZaxEngine::Binding;
#include "imgui.h"

void Component::OnAddToGO()
{
}

void Component::OnGui()
{
	if (onGuiFunc.IsValidate())
	{
		onGuiFunc.Call();
	}
}
