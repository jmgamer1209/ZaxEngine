#pragma once
#include "Component/Component.h"
#include "InspectorEditor.h"

namespace ZaxEngine::Editor
{
	class MeshRendererEditor :InspectorEditor
	{
		public:
			MeshRendererEditor(Component* target);
			void OnGui() override;

	};
}