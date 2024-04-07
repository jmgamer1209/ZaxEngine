#pragma once
#include "Core/ShaderProgram.h"
#include "Assets/AssetModel.h"

class Material
{
public:
	ShaderProgram *shader;
	Material(ShaderProgram* shader) {
		this->shader = shader;
	};

	virtual void Draw(int& texIndex) = 0;
	virtual void OnGui() {};
};

