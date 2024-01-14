#pragma once
#include "glad/gl.h"
#include "Core/Debug.h"

class ShadowFrameBuffer
{
private:
	unsigned int ID;
	unsigned int depthTexture = -1;
	int width;
	int height;

public:
	ShadowFrameBuffer(int width, int height)
	{
		this->width = width;
		this->height = height;

		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
			width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 检查是否完成创建
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Debug::Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}
	~ShadowFrameBuffer()
	{

	}

	unsigned int GetID()
	{
		return ID;
	}

	unsigned int GetDepthTexture()
	{
		return depthTexture;
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }
};
