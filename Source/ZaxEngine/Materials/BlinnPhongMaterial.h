#pragma once
#include "Material.h"
#include "Core/Utils.h"

class  BlinnPhongMaterial : public Material
{
private:
	unsigned int albedoTexture = -1;
	float specular = 0.5f;

public:
	BlinnPhongMaterial(ShaderProgram* shader, std::string albedoTexturePath) : Material(shader)
	{
		Utils::LoadTexture(&albedoTexture, albedoTexturePath);
	}
	~BlinnPhongMaterial() {}

	void Draw(int& texIndex) override
	{
		// ��������Ԫ���ٰ󶨣���ô�����ᱻ����GPUӲ���е�ר�Ŵ洢���������Ԫ��
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
