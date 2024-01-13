#pragma once
#include "glad/gl.h"
#include "Core/Debug.h"

class FrameBuffer
{
private:
	unsigned int ID;
	unsigned int textureColorBuffer = -1;
	unsigned int depthStencilBuffer = -1;
	int width;
	int height;

public:
	FrameBuffer(int width, int height, bool hasDepthStencil = true);
	~FrameBuffer();
	void ChangeSize(int width, int height);
	void ChangeColorTextureFilter(GLenum filter);
	void ChangeColorTextureWrap(GLenum wrap);

	unsigned int GetID()
	{
		return ID;
	}

	unsigned int GetTextureColorBuffer() 
	{
		return textureColorBuffer;
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }
};