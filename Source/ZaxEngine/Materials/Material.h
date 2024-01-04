#pragma once
#include "Core/ShaderProgram.h"
#include "Assets/AssetModel.h"

class Material
{
public:
	Material(ShaderProgram* shader) {
		this->shader = shader;
	};
	ShaderProgram* shader = nullptr;
	//virtual void SetData(AssetModel* model, AssetMesh* mesh) = 0;
	virtual void Draw() = 0;
	virtual void OnGui() {};
};

