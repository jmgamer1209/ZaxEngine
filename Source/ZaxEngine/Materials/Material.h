#pragma once
#include "Core/ShaderProgram.h"
#include "Assets/AssetModel.h"
#include <unordered_map>

enum class MaterialAttributeType;
struct MaterialAttribute;

class Material
{
public:
	unordered_map<string, MaterialAttribute> attributes;
	ShaderProgram *shader;
	Material(ShaderProgram* shader) {
		this->shader = shader;
	};

	bool HasAttribute(const string& name);
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

enum class MaterialAttributeType
{
	Texture, Float
};

struct MaterialAttribute
{
	MaterialAttributeType type;
	union
	{
		MaterialTexture texture;
		float floatValue;
	};

	MaterialAttribute()
	{
		
	};

	MaterialAttribute(const MaterialTexture& v) :texture{ v } 
	{
		type = MaterialAttributeType::Texture;
	};
	MaterialAttribute(const float& v) :floatValue{ v } 
	{
		type = MaterialAttributeType::Float;
	};
};

