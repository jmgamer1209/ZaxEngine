#pragma once
#include "Material.h"
#include "Core/Utils.h"
#include "ThirdParty/imgui/imgui.h"

class ReflectionCubeMaterial : public Material
{
private:
	unsigned int albedoTexture = -1;
	float specular = 0.5f;
	unsigned int environmentTexture = -1;

public:
	ReflectionCubeMaterial(ShaderProgram* shader, AssetModel* model, AssetMesh* mesh, unsigned int environmentTexture) : Material(shader)
	{
		AssetMaterial& assetMat = model->materials[mesh->materialIndex];
		AssetTexture& assetTexture = assetMat.baseColor;
		Utils::LoadTexture(&albedoTexture, assetTexture.path);
		this->environmentTexture = environmentTexture;
	}
	~ReflectionCubeMaterial() {}

	void Draw() override
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoTexture);
		shader->SetUniform("albedoTexture", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, environmentTexture);
		shader->SetUniform("environmentTexture", 1);

		shader->SetUniform("specularIntensity", specular);
		
	}

	void OnGui() override
	{
		ImGui::Text("AlbedoTexID: %d", albedoTexture);
		ImGui::Text("Specular:");
		ImGui::DragFloat("##Specular", &specular, 0.01f, 0, 1);
	}
};