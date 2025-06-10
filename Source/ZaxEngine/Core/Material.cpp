#include "Material.h"
#include "imgui.h"

bool Material::HasProperty(const string& name)
{
	if (properties.find(name) == properties.end()) return false;
	auto att = properties[name];
	if (att.type == MaterialPropertyType::Texture && att.texture->ID == 0) return false;

	return true;
}

void  Material::SetProperty(const string& name, MaterialProperty property)
{
	properties[name] = property;
}

MaterialProperty Material::GetProperty(const string& name)
{
	return properties[name];
}

void  Material::Draw(int& texIndex)
{
	if (m_shadingModel == ShadingModel::BlinnPhong) {
		for (auto pair : properties)
		{
			auto name = pair.first;
			auto value = pair.second;
			if (value.type == MaterialPropertyType::Texture)
			{
				// 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
				auto texture = value.texture;
				if (texture->type == TextureType::Texture2D) {
					glActiveTexture(GL_TEXTURE0 + texIndex);
					glBindTexture(GL_TEXTURE_2D, texture->ID);
					shader->SetUniform(name.c_str(), texIndex);
					texIndex++;
				}
				else if (texture->type == TextureType::CubeMap) {
					glActiveTexture(GL_TEXTURE0 + texIndex);
					glBindTexture(GL_TEXTURE_CUBE_MAP, texture->ID);
					shader->SetUniform("environmentTexture", texIndex);
					texIndex++;
				}
			}
			else if (value.type == MaterialPropertyType::Float)
			{
				shader->SetUniform(name.c_str(), value.floatValue);
			}
			else if (value.type == MaterialPropertyType::Int)
			{
				shader->SetUniform(name.c_str(), value.intValue);
			}
		}
	}
	else {

	}
}

void Material::OnGui()
{
	for (auto pair : properties)
	{
		if (pair.second.type == MaterialPropertyType::Texture)
		{
			ImGui::Text((pair.first + "ID: %d").c_str(), properties[pair.first].texture->ID);
		}
		else if (pair.second.type == MaterialPropertyType::Float)
		{
			ImGui::Text((pair.first + ":").c_str());
			ImGui::DragFloat(("##" + pair.first).c_str(), &properties[pair.first].floatValue, 0.01f, 0, 1);
		}
	}
}
