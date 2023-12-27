#pragma once
#include "Material.h"
#include "../Utils.h"

class  BlinnPhongMaterial : public Material
{
private:
	unsigned int albedoTexture = -1;

public:
	BlinnPhongMaterial(ShaderProgram* shader) : Material(shader) {}
	~BlinnPhongMaterial() {}

	void SetData(AssetModel* model, AssetMesh* mesh) override
	{
		AssetMaterial& assetMat = model->materials[mesh->materialIndex];
		AssetTexture& assetTexture = assetMat.baseColor;

		Utils::LoadTexture(&albedoTexture, assetTexture.path);
	}

	void Draw() override
	{
		// ��������Ԫ���ٰ󶨣���ô�����ᱻ����GPUӲ���е�ר�Ŵ洢���������Ԫ��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoTexture);
		shader->SetUniform("albedoTexture", 0);
	}
};
