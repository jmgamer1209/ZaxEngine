#pragma once
#include <string>
using namespace std;

enum class TextureType: uint8_t
{
	Texture2D = 1, CubeMap = 2
};

class Texture
{
public:
	TextureType type;
	unsigned int ID;

	Texture() { }

	Texture(TextureType type, unsigned int ID);

	static Texture* Load(string& path, TextureType type = TextureType::Texture2D, bool flip_vertically = false);
};

