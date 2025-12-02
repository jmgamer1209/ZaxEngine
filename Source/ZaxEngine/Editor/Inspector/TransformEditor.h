#pragma once
#include "Component/Component.h"
#include "InspectorEditor.h"

namespace ZaxEngine::Editor
{
	class TransformEditor :InspectorEditor
	{
		public:
			TransformEditor(Component* target);
			void OnGui() override;

	};
}