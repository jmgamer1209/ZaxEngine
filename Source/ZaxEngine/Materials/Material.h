#pragma once
#include "Core/ShaderProgram.h"
#include "Assets/AssetModel.h"
#include <unordered_map>
#include "Core/Texture.h"

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

/// <summary>
/// 材质域，目前只有表面
/// </summary>
enum class MaterialDomain {
	Surface = 0
};

/// <summary>
/// 着色模型，比如 blinnphong
/// </summary>
enum class ShadingModel {
	BlinnPhong = 0, 
	ReflectionCube = 1
};

/// <summary>
/// 混合模式，比如透明，不透明
/// </summary>
enum class BlendMode
{
	Opaque = 0, Transparent = 1
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
		Texture texture;
		float floatValue;
		int intValue;
	};

	MaterialProperty()
	{
		
	};

	MaterialProperty(const Texture& v) :texture{ v } 
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

