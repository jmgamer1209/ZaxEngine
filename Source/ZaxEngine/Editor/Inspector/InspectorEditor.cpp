#include "InspectorEditor.h"
#include <imgui.h>
#include <unordered_map>
#include "Component/Camera.h"
#include "CameraEditor.h"
#include <Component/Light.h>
#include "LightEditor.h"
#include "MeshRendererEditor.h"
#include "PostProcessEditor.h"
#include "TransformEditor.h"
#include <Component/MeshRenderer.h>
#include <Component/PostProcess.h>

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

					if (field.TypeNameEqual("System.Int32"))
					{
						int value = field.GetInteger();
						if (ImGui::InputInt(label.c_str(), &value))
						{
							field.SetInteger(value);
						}
					}
					else if (field.TypeNameEqual("System.Boolean"))
					{
						bool value = field.GetBool();
						if (ImGui::Checkbox(label.c_str(), &value))
						{
							field.SetBool(value);
						}
					}
					else if (field.TypeNameEqual("System.Single")) {
						float value = field.GetFloat();
						if (ImGui::InputFloat(label.c_str(), &value))
						{
							field.SetFloat(value);
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
		else if (dynamic_cast<Light*>(component)) {
			return (InspectorEditor*)new LightEditor(component);
		}
		else if (dynamic_cast<MeshRenderer*>(component)) {
			return (InspectorEditor*)new MeshRendererEditor(component);
		}
		else if (dynamic_cast<PostProcess*>(component)) {
			return (InspectorEditor*)new PostProcessEditor(component);
		}
		else if (dynamic_cast<Transform*>(component)) {
			return (InspectorEditor*)new TransformEditor(component);
		}
		else {
			return new InspectorEditor(component); // 或返回默认编辑器
		}
	}

	static GameObject* drawGO = nullptr;
	static unordered_map<Component*, InspectorEditor*> drawEditorMap = {};
	void InspectorEditor::DrawInspector(GameObject& go)
	{
		if (drawGO != &go) {
			drawGO = &go;
			for (const auto &pair : drawEditorMap)
			{
				delete pair.second;
			}
			drawEditorMap.clear();
		}
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