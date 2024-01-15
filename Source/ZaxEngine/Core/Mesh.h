#pragma once
#include <vector>
#include "Core/Vector.h"
#include "Core/Vertex.h"
#include "glad/gl.h"
using namespace std;

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	bool hasNormal = true;
	bool hasTexCoord = true;

private:
	unsigned int VBO, EBO, VAO;
	bool isGenDrawData = false;


private:
	void ReleaseDrawData()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		isGenDrawData = false;
	}

public:
	~Mesh()
	{
		ReleaseDrawData();
	}

	void GenDrawData()
	{
		if (isGenDrawData == true) return;
		isGenDrawData = true;
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		if (indices.size() > 0) glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		if (indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		}

		int indexVA = 0;
		glEnableVertexAttribArray(indexVA);
		glVertexAttribPointer(indexVA, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		indexVA++;

		if (hasNormal) 
		{
			glEnableVertexAttribArray(indexVA);
			glVertexAttribPointer(indexVA, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			indexVA++;
		}
		
		if (hasTexCoord) 
		{
			glEnableVertexAttribArray(indexVA);
			glVertexAttribPointer(indexVA, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
			indexVA++;
		}

		// 切线和副切线暂时不用
		//// vertex tangent
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//// vertex bitangent
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}

	unsigned int GetVAO()
	{
		return VAO;
	}
	
	void Draw()
	{
		glBindVertexArray(VAO);

		if (indices.size() > 0)
		{
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		}
		else glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
	}

private:
	static Mesh* quadMesh;
	static vector<Vector3> quadVertices; 

public:
	static Mesh* GetQuadMesh()
	{
		if (Mesh::quadMesh == nullptr)
		{
			Mesh::quadMesh = new Mesh();
			Mesh::quadMesh->hasNormal = false;
			auto vertex1 = Vertex(Vector3(-1,-1,0), Vector3(), Vector2(0,0));
			auto vertex2 = Vertex(Vector3(1, -1, 0), Vector3(), Vector2(1, 0));
			auto vertex3 = Vertex(Vector3(1, 1, 0), Vector3(), Vector2(1, 1));
			auto vertex4 = Vertex(Vector3(-1, -1, 0), Vector3(), Vector2(0, 0));
			auto vertex5 = Vertex(Vector3(1, 1, 0), Vector3(), Vector2(1, 1));
			auto vertex6 = Vertex(Vector3(-1, 1, 0), Vector3(), Vector2(0, 1));
			
			Mesh::quadMesh->vertices =
			{
					vertex1,
					vertex2,
					vertex3,
					vertex4,
					vertex5,
					vertex6,
			};
			Mesh::quadMesh->GenDrawData();
		}
		return Mesh::quadMesh;
	}
};