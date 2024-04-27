#include "Material.h"

bool Material::HasProperty(const string& name)
{
	if (properties.find(name) == properties.end()) return false;
	auto att = properties[name];
	if (att.type == MaterialPropertyType::Texture && att.texture.ID == 0) return false;

	return true;
}

void  Material::SetProperty(const string& name, MaterialProperty property)
{
	properties[name] = property;
}

MaterialProperty Material::GetProperty(const string& name)
{
	return properties[name];
}