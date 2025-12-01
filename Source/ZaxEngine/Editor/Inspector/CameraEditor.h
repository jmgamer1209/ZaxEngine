#pragma once
#include "Component/Component.h"
#include "InspectorEditor.h"

namespace ZaxEngine::Editor
{
	class CameraEditor:InspectorEditor
	{
		public:
			CameraEditor(Component* target);
			void OnGui() override;

	};
}