#pragma once

#include "glad/gl.h"
#include "Core/Debug.h"

class ShadowFrameBufferBase
{
protected:
	unsigned int ID;
	int width;
	int height;

public:
	ShadowFrameBufferBase(int width, int height)
	{
		this->width = width;
		this->height = height;
	}
	~ShadowFrameBufferBase()
	{

	}

	unsigned int GetID()
	{
		return ID;
	}

	virtual unsigned int GetBindTexture()
	{
		return -1;
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }
};

