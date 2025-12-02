#pragma once
#include "Component/Component.h"
#include "InspectorEditor.h"

namespace ZaxEngine::Editor
{
	class LightEditor:InspectorEditor
	{
		public:
			LightEditor(Component* target);
			void OnGui() override;

	};
}