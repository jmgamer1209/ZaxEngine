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
	 // 激活纹理单元后，再绑定，那么纹理将会被放在GPU硬件中的专门存储纹理的纹理单元中
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->SetUniform("texture1", 0);
 }



