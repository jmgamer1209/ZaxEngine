#pragma once
#include "Component.h"
#include "CS/Bindings/BindingHelper.h"
using namespace ZaxEngine::Binding;
#include "imgui.h"

void Component::OnGui()
{
	if (scriptObj.IsValidate()) {
		auto name = scriptObj.GetName();
		if (ImGui::TreeNode(name)) {
			auto fields = scriptObj.GetInspectorFields();

			for (size_t i = 0; i < fields.size(); i++)
			{
				auto field = fields[i];
				auto fieldName = field.GetName();
				ImGui::Text(fieldName);
				ImGui::SameLine(0,-2);
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

				

				
				//ImGui::InputInt
			}
			
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
