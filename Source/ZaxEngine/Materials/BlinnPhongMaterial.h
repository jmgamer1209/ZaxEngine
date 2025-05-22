#pragma once
#include "Material.h"
#include "Core/Utils.h"

class  BlinnPhongMaterial : public Material
{
public:

	BlinnPhongMaterial(ShaderProgram* shader, AssetMaterial* mat) : Material(shader)
	{
		unsigned int albedoTexture = 0;
		unsigned int normalMap = 0;
		Utils::LoadTexture(&albedoTexture, mat->baseColor.path);
		Utils::LoadTexture(&normalMap, mat->normal.path);

		properties["AlbedoTexture"] = MaterialProperty(Texture(TextureType::Texture2D, albedoTexture));
		if (normalMap != 0) properties["NormalMap"] = MaterialProperty(Texture(TextureType::Texture2D, normalMap));
		properties["Specular"] = MaterialProperty(0.5f);
	}

	BlinnPhongMaterial(ShaderProgram* shader, std::string albedoTexturePath) : Material(shader)
	{
		unsigned int albedoTexture = 0; 
		Utils::LoadTexture(&albedoTexture, albedoTexturePath);
		properties["AlbedoTexture"] = MaterialProperty(Texture(TextureType::Texture2D, albedoTexture));
		properties["Specular"] = MaterialProperty(0.5f);
	}
	~BlinnPhongMaterial() {}

	void Draw(int& texIndex) override
	{
		for (auto pair:properties)
		{
			if (pair.second.type == MaterialPropertyType::Texture)
			{
				// 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
				glActiveTexture(GL_TEXTURE0 + texIndex);
				glBindTexture(GL_TEXTURE_2D, GetProperty(pair.first).texture.ID);
				shader->SetUniform(pair.first.c_str(), texIndex);
				texIndex++;
			}
			else if (pair.second.type == MaterialPropertyType::Float)
			{
				shader->SetUniform(pair.first.c_str(), GetProperty(pair.first).floatValue);
			}
			else if (pair.second.type == MaterialPropertyType::Int)
			{
				shader->SetUniform(pair.first.c_str(), GetProperty(pair.first).intValue);
			}
		}
	}

	void OnGui() override
	{
		for (auto pair : properties)
		{
			if (pair.second.type == MaterialPropertyType::Texture)
			{
				ImGui::Text((pair.first+"ID: %d").c_str(), properties[pair.first].texture.ID);
			}
			else if (pair.second.type == MaterialPropertyType::Float)
			{
				ImGui::Text((pair.first+":").c_str());
				ImGui::DragFloat(("##"+pair.first).c_str(), &properties[pair.first].floatValue, 0.01f, 0, 1);
			}
		}

	}
};
