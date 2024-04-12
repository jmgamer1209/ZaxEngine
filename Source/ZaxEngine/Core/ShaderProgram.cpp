#include "ShaderProgram.h"
#include "Debug.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Node
{
public:
	int value = 0;
	vector<Node*> childs;

	Node()
	{

	}

	Node(Node* parent)
	{
		parent->childs.push_back(this);
	}
};

class KeywordAndShaderPart
{
public:
	vector<string> keywords;
	std::stringstream versionStream;
	std::stringstream contentStream;
};

void CheckCompileError(unsigned int shader, string type);
void GenerateKeywordTree(Node* parent, int start, int end);
void GenerateKeywordGroup(Node* node, vector<Node*> pre, vector<string>& keywords, vector<ShaderWithKeywords*>& result);
void GenerateProgram(KeywordAndShaderPart* vertexPart, KeywordAndShaderPart* fragmentPart, vector<ShaderWithKeywords*>& shaders);
int CompileShader(vector<string>& keywords, std::stringstream& shaderTopStream, std::stringstream& shaderBottomStream, int type);

ShaderProgram::ShaderProgram(string vertexPath, string fragmentPath)
{
	KeywordAndShaderPart* vertexPart = LoadShader(vertexPath);
	KeywordAndShaderPart* fragmentPart = LoadShader(fragmentPath);

	GenerateProgram(vertexPart, fragmentPart, shaders);

	delete vertexPart;
	delete fragmentPart;

	currentShader = shaders[0];
}

void GenerateProgram(KeywordAndShaderPart* vertexPart, KeywordAndShaderPart* fragmentPart, vector<ShaderWithKeywords*>& shaders)
{
	auto keywords = vector<string>();
	keywords.insert(keywords.end(), vertexPart->keywords.begin(), vertexPart->keywords.end());
	keywords.insert(keywords.end(), fragmentPart->keywords.begin(), fragmentPart->keywords.end());

	// 去除相同的 keyword
	for (int i = 0; i < (int)keywords.size() - 1; i++)
	{
		for (int j = i+1; j < (int)keywords.size(); j++)
		{
			if (keywords[i] == keywords[j])
			{
				keywords.erase(keywords.begin()+j);
				j--;
			}
		}
	}

	shaders.push_back(new ShaderWithKeywords()); // 添加空的 keyword 列表，作为默认 shader
	auto root = new Node();
	GenerateKeywordTree(root, 0, (int)keywords.size() - 1);
	GenerateKeywordGroup(root, vector<Node*>(), keywords, shaders);

	for (size_t i = 0; i < shaders.size(); i++)
	{
		auto vertexID = CompileShader(shaders[i]->keywords, vertexPart->versionStream, vertexPart->contentStream, 1);
		auto fragmentID = CompileShader(shaders[i]->keywords, fragmentPart->versionStream, fragmentPart->contentStream, 2);

		auto ID = glCreateProgram();
		glAttachShader(ID, vertexID);
		glAttachShader(ID, fragmentID);
		glLinkProgram(ID);
		CheckCompileError(ID, "PROGRAM");

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		shaders[i]->ID = ID;
	}
}

int CompileShader(vector<string>& keywords, std::stringstream& versionStream, std::stringstream& contentStream, int type)
{
	std::string str = versionStream.str();
	
	str += "\n";
	for (size_t i = 0; i < keywords.size(); i++)
	{
		str += "#define ";
		str += keywords[i];
		str += "\n";
	}
	str += "\n";

	str.append(contentStream.str());

	auto vShaderCode = str.c_str();

	unsigned int ID;
	ID = glCreateShader(type == 1 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	glShaderSource(ID, 1, &vShaderCode, NULL);
	glCompileShader(ID);
	CheckCompileError(ID, type == 1 ? "VERTEX" : "FRAGMENT");
	return ID;
}

KeywordAndShaderPart* ShaderProgram::LoadShader(string path)
{
	auto shaderPart = new KeywordAndShaderPart();
	std::string shaderCodeLine;
	std::ifstream shaderFile;

	shaderFile.open(path);

	// 通过状态机，分析文本
	// 进入 keyword 状态，当一行不是空格且不是keyword，则退出判断，keyword判断结束，开始构成真正的shader文本
	// 目前任意 keyword 之间，都不允许有其他文本存在
	int stat = 0;
	string keyword;
	while (getline(shaderFile, shaderCodeLine))
	{
		switch (stat)
		{
		case 0: // 找 version
		{
			string word = "";
			bool start = false;
			for (size_t i = 0; i < shaderCodeLine.size(); i++)
			{
				auto ch = shaderCodeLine[i];
				if (ch == ' ') continue;
				else if (start == false)
				{
					start = true;
					word += ch;
				}
				else if (start == true)
				{
					if (ch != ' ') word += ch;
					else break;
				}
			}

			if (word == "#version")
			{
				shaderPart->versionStream << shaderCodeLine << '\n';
				stat = 1;
				break;
			}
			else if (word == "")
			{
				break;
			}
			else
			{
				shaderPart->versionStream << shaderCodeLine << '\n';
				stat = 1;
				break;
			}
		}
		case 1: // 找 Keyword
		{
			if (shaderCodeLine.length() < 10)
			{
				shaderPart->contentStream << shaderCodeLine << '\n';
				break;
			}
			keyword = shaderCodeLine.substr(0, 10);
			if (keyword == "//KEYWORD:")
			{
				stat = 2;
				keyword = shaderCodeLine.substr(10, shaderCodeLine.length() - 10);
				shaderPart->keywords.push_back(keyword);
			}
			else
			{
				shaderPart->contentStream << shaderCodeLine << '\n';
			}
			break;
		}
		case 2:
		{
			if (shaderCodeLine.length() < 10)
			{
				shaderCodeLine.erase(std::remove_if(shaderCodeLine.begin(), shaderCodeLine.end(), isspace), shaderCodeLine.end());
				if (shaderCodeLine.size() <= 0) break;
				else
				{
					shaderPart->contentStream << shaderCodeLine << '\n';
					stat = 3;
					break;
				}
			}
			keyword = shaderCodeLine.substr(0, 10);
			if (keyword == "//KEYWORD:")
			{
				keyword = shaderCodeLine.substr(10, shaderCodeLine.length() - 10);
				shaderPart->keywords.push_back(keyword);
			}
			else
			{
				shaderPart->contentStream << shaderCodeLine << '\n';
				stat = 3;
				break;
			}
			break;
		}
		default:
			break;
		}

		if (stat == 3) break;
	}

	shaderPart->contentStream << shaderFile.rdbuf();   // read file's buffer contents into streams
	shaderFile.close();

	return shaderPart;
}

void GenerateKeywordTree(Node* parent, int start, int end)
{
	for (int i = start; i <= end; i++)
	{
		auto node = new Node(parent);
		node->value = i;
		GenerateKeywordTree(node, i + 1, end);
	}
}

void GenerateKeywordGroup(Node* node, vector<Node*> pre, vector<string>& keywords, vector<ShaderWithKeywords*>& result)
{
	for (int i = 0; i < node->childs.size(); i++)
	{
		auto list = vector<Node*>(pre);
		list.push_back(node->childs[i]);

		ShaderWithKeywords* shader = new ShaderWithKeywords();
		for (Node* item : list)
		{
			shader->keywords.push_back(keywords[item->value]);
		}
		result.push_back(shader);
		GenerateKeywordGroup(node->childs[i], list, keywords, result);
	}
}

void CheckCompileError(unsigned int shader, string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Debug::Log({ "ERROR::Shader_Complication Error of type: ", type, "\\n", infoLog , "\\n -- --------------------------------------------------- --" });
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Debug::Log({ "ERROR::ProgramLink Error of type: ", type, "\\n", infoLog , "\\n -- --------------------------------------------------- --" });
		}
	}
}

void ShaderProgram::Delete()
{
	for (size_t i = 0; i < shaders.size(); i++)
	{
		glDeleteProgram(shaders[i]->ID);
	}
}

void ShaderProgram::Use()
{
	glUseProgram(currentShader->ID);
}

void ShaderProgram::EnableKeyword(string keyword)
{
	if (keywords.count(keyword) == 0)
	{
		keywords.insert(keyword);
		SetShaderWithKeywords();
	}
}

void ShaderProgram::DisableKeyword(string keyword)
{
	if (keywords.count(keyword) != 0)
	{
		keywords.erase(keyword);
		SetShaderWithKeywords();
	}
}

void ShaderProgram::SetShaderWithKeywords()
{
	std::unordered_map<string, int> map;
	for (auto it : keywords)
	{
		map[it] = 0;
	}

	for (size_t i = 0; i < shaders.size(); i++)
	{
		ShaderWithKeywords* shader = shaders[i];
		bool find = true;
		if (keywords.size() != shader->keywords.size()) continue;
		for (size_t j = 0; j < shader->keywords.size(); j++)
		{
			if (map.count(shader->keywords[j]) < 0 || map[shader->keywords[j]] == 1)
			{
				find = false;
				break;
			}
			else
			{
				map[shader->keywords[j]] = 1;
			}
		} 

		if (find)
		{
			currentShader = shader;
			break;
		}
		else
		{
			for (auto it : map) {
				it.second = 0;
			}
		}
	}
}

void ShaderProgram::SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform4f(location, v0, v1, v2, v3);
}

void ShaderProgram::SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform3f(location, v0, v1, v2);
}

void ShaderProgram::SetUniform(const GLchar* name, glm::vec3 value)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform3f(location, value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const GLchar* name, Vector3 value)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform3f(location, value.x, value.y, value.z);
}

void ShaderProgram::SetUniform3f(const GLchar* name, float value[])
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform3f(location, value[0], value[1], value[2]);
}

void ShaderProgram::SetUniform(const GLchar* name, GLint v0)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform1i(location, v0);
}

void ShaderProgram::SetUniform(const GLchar* name, GLfloat v0)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniform1f(location, v0);
}

void ShaderProgram::SetUniform(const GLchar* name, glm::mat4& mat)
{
	int location = glGetUniformLocation(currentShader->ID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}