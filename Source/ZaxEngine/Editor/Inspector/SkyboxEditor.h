#pragma once
#include "Component/Component.h"
#include "InspectorEditor.h"

namespace ZaxEngine::Editor
{
	class SkyboxEditor :InspectorEditor
	{
		public:
			SkyboxEditor(Component* target);
			void OnGui() override;

	};
}