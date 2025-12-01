#include "InspectorEditor.h"
#include <imgui.h>
#include <unordered_map>
#include "Component/Camera.h"
#include "CameraEditor.h"

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

					
				}

				ImGui::TreePop();
			}
		}
	}


	InspectorEditor* GetInspectorEditor(Component* component)
	{
		// 判断 component 和 CameraEditor* 类型是否相同
		if (dynamic_cast<Camera*>(component)) {
			return (InspectorEditor*)new CameraEditor(component);
		}
		else {
			return new InspectorEditor(component); // 或返回默认编辑器
		}
		
	}

	static GameObject* drawGO = nullptr;
	static unordered_map<Component*, InspectorEditor*> drawEditorMap = {};
	void InspectorEditor::DrawInspector(GameObject& go)
	{
		for (size_t i = 0; i < go.components.size(); i++)
		{
			Component* component = go.components[i];
			auto editor = drawEditorMap[component];
			if (editor == nullptr) {
				editor = GetInspectorEditor(component);
				drawEditorMap[component] = editor;
			}
			editor->OnGui();
		}
	}
}