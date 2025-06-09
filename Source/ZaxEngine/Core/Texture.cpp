#include "Texture.h"
#include "Core/Utils.h"

Texture::Texture(TextureType type, unsigned int ID)
{
	this->type = type;
	this->ID = ID;
}

Texture Texture::Load(string& path, TextureType type, bool flip_vertically)
{
	if (type == TextureType::Texture2D) {
		return Utils::LoadTexture(path, flip_vertically);
	}
	else {
		auto cubemap = Utils::LoadCubeMap(path);
		Texture(TextureType::CubeMap, cubemap);
	}
}