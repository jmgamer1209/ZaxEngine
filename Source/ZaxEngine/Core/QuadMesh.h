#pragma once
#include "glad/gl.h"

class QuadMesh
{
private:
	float vertices[30] = {
			-1, -1, 0, 0, 0,
			1, -1, 0, 1, 0,
			1,  1, 0, 1, 1,
			-1,-1, 0, 0, 0,
			1,  1, 0, 1, 1,
			-1, 1, 0, 0, 1
	};
	unsigned int VBO;

public:
	QuadMesh()
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Setup()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};