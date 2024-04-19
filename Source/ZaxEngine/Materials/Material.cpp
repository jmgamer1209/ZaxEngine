#include "Material.h"

bool Material::HasAttribute(const string& name)
{
	if (attributes.find(name) == attributes.end()) return false;
	auto att = attributes[name];
	if (att.type == MaterialAttributeType::Texture && att.texture.ID == 0) return false;

	return true;
}