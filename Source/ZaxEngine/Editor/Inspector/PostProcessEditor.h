#pragma once
#include "Component/Component.h"
#include "InspectorEditor.h"

namespace ZaxEngine::Editor
{
	class PostProcessEditor :InspectorEditor
	{
		public:
			PostProcessEditor(Component* target);
			void OnGui() override;

	};
}