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

		attributes["albedoTexture"] = MaterialAttribute(MaterialTexture(TextureType::Texture2D, albedoTexture));
		if (normalMap != 0) attributes["normalMap"] = MaterialAttribute(MaterialTexture(TextureType::Texture2D, normalMap));
		attributes["specular"] = MaterialAttribute(0.5f);
	}

	BlinnPhongMaterial(ShaderProgram* shader, std::string albedoTexturePath) : Material(shader)
	{
		unsigned int albedoTexture = 0;
		Utils::LoadTexture(&albedoTexture, albedoTexturePath);
		attributes["albedoTexture"] = MaterialAttribute(MaterialTexture(TextureType::Texture2D, albedoTexture));
		attributes["specular"] = MaterialAttribute(0.5f);
	}
	~BlinnPhongMaterial() {}

	void Draw(int& texIndex) override
	{
		// 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
		glActiveTexture(GL_TEXTURE0+ texIndex);
		glBindTexture(GL_TEXTURE_2D, attributes["albedoTexture"].texture.ID);
		shader->SetUniform("albedoTexture", texIndex);
		texIndex++; 

		if (attributes.count("normalMap") != 0)
		{
			glActiveTexture(GL_TEXTURE0 + texIndex);
			glBindTexture(GL_TEXTURE_2D, attributes["normalMap"].texture.ID);
			shader->SetUniform("normalMap", texIndex);
			texIndex++;
		}

		shader->SetUniform("specularIntensity", attributes["specular"].floatValue);
	}

	void OnGui() override
	{
		ImGui::Text("AlbedoTexID: %d", attributes["albedoTexture"].texture.ID);
		ImGui::Text("Specular:");
		ImGui::DragFloat("##Specular", &attributes["specular"].floatValue, 0.01f, 0, 1);
	}
};
