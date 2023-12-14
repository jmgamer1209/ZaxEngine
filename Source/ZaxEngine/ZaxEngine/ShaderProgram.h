#pragma once
#include "glad/glad.h"
#include <string>
using namespace std;

class ShaderProgram
{
	unsigned int ID;
	
public:
	ShaderProgram(string vertexPath, string fragmentPath);
	void Delete();
	void Use();

	void SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);

private:
	unsigned int LoadShader(string path, int type);
	void CheckCompileError(unsigned int shader, string type);
};

