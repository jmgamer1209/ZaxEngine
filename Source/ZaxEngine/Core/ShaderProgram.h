#pragma once
#include "boost/filesystem/path.hpp"
#include "glad/gl.h"
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Vector.h"
#include <vector>
#include <unordered_set>


using namespace std;

class ShaderWithKeywords
{
public:
	vector<string> keywords;
	unsigned int ID = 0;

	ShaderWithKeywords()
	{

	}
};

class KeywordAndShaderPart;

class ShaderProgram
{
	vector<ShaderWithKeywords*> shaders;
	ShaderWithKeywords* currentShader;
	unordered_set<string> keywords;
	
public:
	ShaderProgram(boost::filesystem::path vertexPath, boost::filesystem::path fragmentPath);
	void Delete();
	void Use();
	void EnableKeyword(string keyword);
	void DisableKeyword(string keyword);

	void SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
	void SetUniform(const GLchar* name, glm::vec3 value);
	void SetUniform(const GLchar* name, Vector3 value);
	void SetUniform3f(const GLchar* name, float value[]);
	void SetUniform(const GLchar* name, GLint v0);
	void SetUniform(const GLchar* name, GLfloat v0);
	void SetUniform(const GLchar* name, glm::mat4& mat);

private:
	KeywordAndShaderPart* LoadShader(boost::filesystem::path path);
	void SetShaderWithKeywords();
};

class ShaderProgramManager
{
	
};
