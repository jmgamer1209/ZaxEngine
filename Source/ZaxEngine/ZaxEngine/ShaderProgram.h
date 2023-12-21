#pragma once
#include "glad/glad.h"
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	void SetUniform(const GLchar* name, GLint v0);
	void SetUniform(const GLchar* name, glm::mat4& mat);

private:
	unsigned int LoadShader(string path, int type);
	void CheckCompileError(unsigned int shader, string type);
};

class ShaderProgramManager
{
	
};
