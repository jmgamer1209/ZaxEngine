#pragma once

enum class TextureType
{
	Texture2D, CubeMap
};

struct Texture
{
	TextureType type;
	unsigned int ID;

	Texture(TextureType type, unsigned int ID)
	{
		this->type = type;
		this->ID = ID;
	}
};

