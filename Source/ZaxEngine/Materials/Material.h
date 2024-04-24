#pragma once
#include "Core/ShaderProgram.h"
#include "Assets/AssetModel.h"
#include <unordered_map>

enum class MaterialPropertyType;
struct MaterialProperty;

class Material
{
protected:
	unordered_map<string, MaterialProperty> properties;

public:
	ShaderProgram *shader;
	Material(ShaderProgram* shader) {
		this->shader = shader;
	};

	bool HasProperty(const string& name);
	void SetProperty(const string& name, MaterialProperty property);
	virtual void Draw(int& texIndex) = 0;
	virtual void OnGui() {};
};

enum class TextureType
{
	Texture2D, CubeMap
};

struct  MaterialTexture
{
	TextureType type;
	unsigned int ID;

	MaterialTexture(TextureType type, unsigned int ID)
	{
		this->type = type;
		this->ID = ID;
	}
};

enum class MaterialPropertyType
{
	Texture, Float
};

struct MaterialProperty
{
	MaterialPropertyType type;
	union
	{
		MaterialTexture texture;
		float floatValue;
	};

	MaterialProperty()
	{
		
	};

	MaterialProperty(const MaterialTexture& v) :texture{ v } 
	{
		type = MaterialPropertyType::Texture;
	};
	MaterialProperty(const float& v) :floatValue{ v } 
	{
		type = MaterialPropertyType::Float;
	};
};

