#pragma once
#include "Core/QuadMesh.h"

class Mesh
{
private:
	static QuadMesh* quadMesh;

public:
	static QuadMesh* GetQuadMesh()
	{
		if (Mesh::quadMesh == nullptr)
		{
			Mesh::quadMesh = new QuadMesh();
		}
		return Mesh::quadMesh;
	}
};