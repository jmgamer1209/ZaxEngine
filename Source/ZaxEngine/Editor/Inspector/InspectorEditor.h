#pragma once
#include "Component/Component.h"

namespace ZaxEngine::Editor
{
	class InspectorEditor
	{
		public:
			Component* target;

			InspectorEditor(Component* target);

			virtual void OnGui();

			void static DrawInspector(GameObject& go);
	};
}