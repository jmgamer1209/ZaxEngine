#pragma once
#include "Material.h"

class  DefaultMaterial: public Material
{
private:
	unsigned int albedoTexture = -1;

public:
	 DefaultMaterial(ShaderProgram* shader);
	~ DefaultMaterial();

	void SetData(AssetModel* model, AssetMesh* mesh) override;
	void Draw() override;
};

 DefaultMaterial::DefaultMaterial(ShaderProgram *shader): Material(shader)
{
	
}

 DefaultMaterial::~ DefaultMaterial()
{

}

 void DefaultMaterial::SetData(AssetModel* model, AssetMesh* mesh)
 {
	 AssetMaterial& assetMat = model->materials[mesh->materialIndex];
	 AssetTexture& assetTexture = assetMat.baseColor;

	 Utils::LoadTexture(&albedoTexture, assetTexture.path);
 }

 void DefaultMaterial::Draw()
 {
	 // 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedoTexture);
	shader->SetUniform("albedoTexture", 0);
 }



