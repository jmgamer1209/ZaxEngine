#pragma once
#include "Material.h"
#include "Core/Utils.h"

class  DefaultMaterial: public Material
{
private:
	unsigned int albedoTexture = -1;

public:
	 DefaultMaterial(ShaderProgram* shader, AssetModel* model, AssetMesh* mesh);
	~ DefaultMaterial();

//	void SetData(AssetModel* model, AssetMesh* mesh) override;
	void Draw() override;
};

 DefaultMaterial::DefaultMaterial(ShaderProgram *shader, AssetModel* model, AssetMesh* mesh): Material(shader)
{
	 AssetMaterial& assetMat = model->materials[mesh->materialIndex];
	 AssetTexture& assetTexture = assetMat.baseColor;
	 Utils::LoadTexture(&albedoTexture, assetTexture.path);
}

 DefaultMaterial::~ DefaultMaterial()
{

}

 //void DefaultMaterial::SetData(AssetModel* model, AssetMesh* mesh)
 //{

 //}

 void DefaultMaterial::Draw()
 {
	 // ��������Ԫ���ٰ󶨣���ô�����ᱻ����GPUӲ���е�ר�Ŵ洢���������Ԫ��
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedoTexture);
	shader->SetUniform("albedoTexture", 0);
 }



