#include "InspectorEditor.h"
#include <imgui.h>
#include <unordered_map>

namespace ZaxEngine::Editor
{
	InspectorEditor::InspectorEditor(Component* target)
	{
		this->target = target;
	}

	void InspectorEditor::OnGui()
	{
		auto scriptObj = target->scriptObj;
		if (scriptObj.IsValidate()) {
			auto name = scriptObj.GetName();
			if (ImGui::TreeNode(name)) {
				auto fields = scriptObj.GetInspectorFields();

				for (size_t i = 0; i < fields.size(); i++)
				{
					auto field = fields[i];
					auto fieldName = field.GetName();
					ImGui::Text(fieldName);
					ImGui::SameLine(0, -2);
					std::string label = string("##") + string(fieldName);

					if (strcmp(field.GetTypeName(), "System.Int32") == 0)
					{
						int value = field.GetInteger();

						if (ImGui::InputInt(label.c_str(), &value))
						{
							field.SetInteger(value);
						}
					}
					else {
						ImGui::Text(field.GetTypeName());
					}




					//			//ImGui::InputInt
					//		}

							//ImGui::int
							/*ImGui::DragFloat("##Fov-Vertical", &this->fovy, 1.0f, 0.1f, 179.0f);
							ImGui::Text("Near:");
							ImGui::DragFloat("##Near", &this->clippingNear, 1.0f, 0, (float)INT32_MAX);
							ImGui::Text("Far:");
							ImGui::DragFloat("##Far", &this->clippingFar, 1.0f, 0, (float)INT32_MAX);*/

					ImGui::TreePop();
				}
			}
		}
	}

	static GameObject* drawGO = nullptr;
	static unordered_map<Component*, InspectorEditor*> drawEditorMap = {};
	void InspectorEditor::DrawInspector(GameObject& go)
	{
		for (size_t i = 0; i < go.components.size(); i++)
		{
			Component* component = go.components[i];
			InspectorEditor* editor = new InspectorEditor(component);
		}
	}
}