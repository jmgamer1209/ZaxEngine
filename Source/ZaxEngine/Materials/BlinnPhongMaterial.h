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

		properties["albedoTexture"] = MaterialProperty(MaterialTexture(TextureType::Texture2D, albedoTexture));
		if (normalMap != 0) properties["normalMap"] = MaterialProperty(MaterialTexture(TextureType::Texture2D, normalMap));
		properties["specular"] = MaterialProperty(0.5f);
	}

	BlinnPhongMaterial(ShaderProgram* shader, std::string albedoTexturePath) : Material(shader)
	{
		unsigned int albedoTexture = 0;
		Utils::LoadTexture(&albedoTexture, albedoTexturePath);
		properties["albedoTexture"] = MaterialProperty(MaterialTexture(TextureType::Texture2D, albedoTexture));
		properties["specular"] = MaterialProperty(0.5f);
	}
	~BlinnPhongMaterial() {}

	void Draw(int& texIndex) override
	{
		// 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
		glActiveTexture(GL_TEXTURE0+ texIndex);
		glBindTexture(GL_TEXTURE_2D, properties["albedoTexture"].texture.ID);
		shader->SetUniform("albedoTexture", texIndex);
		texIndex++; 

		if (properties.count("normalMap") != 0)
		{
			glActiveTexture(GL_TEXTURE0 + texIndex);
			glBindTexture(GL_TEXTURE_2D, properties["normalMap"].texture.ID);
			shader->SetUniform("normalMap", texIndex);
			texIndex++;
		}

		shader->SetUniform("specularIntensity", properties["specular"].floatValue);
	}

	void OnGui() override
	{
		ImGui::Text("AlbedoTexID: %d", properties["albedoTexture"].texture.ID);
		ImGui::Text("Specular:");
		ImGui::DragFloat("##Specular", &properties["specular"].floatValue, 0.01f, 0, 1);
	}
};
