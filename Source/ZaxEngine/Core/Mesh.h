#pragma once
#include <vector>
#include "Core/QuadMesh.h"
#include "Core/Vector.h"
#include "Core/Vertex.h"
using namespace std;

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;



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