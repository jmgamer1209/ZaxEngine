#pragma once
#include "Material.h"
#include "Core/Utils.h"

class  BlinnPhongMaterial : public Material
{
private:
	unsigned int albedoTexture = 0;
	unsigned int normalMap = 0;
	float specular = 0.5f;

public:

	BlinnPhongMaterial(ShaderProgram* shader, AssetMaterial* mat) : Material(shader)
	{
		Utils::LoadTexture(&albedoTexture, mat->baseColor.path);
		Utils::LoadTexture(&normalMap, mat->normal.path);
	}

	BlinnPhongMaterial(ShaderProgram* shader, std::string albedoTexturePath) : Material(shader)
	{
		Utils::LoadTexture(&albedoTexture, albedoTexturePath);
	}
	~BlinnPhongMaterial() {}

	void Draw(int& texIndex) override
	{
		// 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
		glActiveTexture(GL_TEXTURE0+ texIndex);
		glBindTexture(GL_TEXTURE_2D, albedoTexture);
		shader->SetUniform("albedoTexture", texIndex);
		texIndex++;
		shader->SetUniform("specularIntensity", specular);
	}

	void OnGui() override
	{
		ImGui::Text("AlbedoTexID: %d", albedoTexture);
		ImGui::Text("Specular:");
		ImGui::DragFloat("##Specular", &specular, 0.01f, 0, 1);
	}
};
