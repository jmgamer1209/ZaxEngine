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
	MaterialProperty GetProperty(const string& name);
	virtual void Draw(int& texIndex) = 0;
	virtual void OnGui() {};
};

enum class SurfaceType
{
	Opaque = 0, Transparent = 1
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
	Texture, Float, Int
};

struct MaterialProperty
{
	MaterialPropertyType type;
	union
	{
		MaterialTexture texture;
		float floatValue;
		int intValue;
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

	MaterialProperty(const int& v) :intValue{ v }
	{
		type = MaterialPropertyType::Int;
	};
};

