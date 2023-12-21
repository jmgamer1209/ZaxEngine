#pragma once
#include "Material.h"

class  DefaultMaterial: public Material
{
public:
	 DefaultMaterial();
	~ DefaultMaterial();

	void SetData(AssetModel* model, AssetMesh* mesh) override;
	void Draw() override;

private:
	unsigned int texture = -1;
};

 DefaultMaterial:: DefaultMaterial()
{
}

 DefaultMaterial::~ DefaultMaterial()
{

}

 void DefaultMaterial::SetData(AssetModel* model, AssetMesh* mesh)
 {
	 AssetMaterial& assetMat = model->materials[mesh->materialIndex];
	 AssetTexture& assetTexture = assetMat.baseColor;

	 Utils::LoadTexture(&texture, assetTexture.path);
 }

 void DefaultMaterial::Draw()
 {
	 // ��������Ԫ���ٰ󶨣���ô�����ᱻ����GPUӲ���е�ר�Ŵ洢���������Ԫ��
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->SetUniform("texture1", 0);
 }



