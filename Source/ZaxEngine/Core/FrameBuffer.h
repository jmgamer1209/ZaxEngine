#pragma once
#include "glad/gl.h"
#include "Core/Debug.h"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();
	void ChangeSize(int width, int height);

private:
	unsigned int ID;
	unsigned int textureColorBuffer;
	unsigned int rbo;
	int width;
	int height;

public:
	unsigned int GetID()
	{
		return ID;
	}

	unsigned int GetTextureColorBuffer() 
	{
		return textureColorBuffer;
	}
};