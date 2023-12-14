#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(string vertexPath, string fragmentPath)
{
    auto vertex = LoadShader(vertexPath, 1);
    auto fragment = LoadShader(fragmentPath, 2);
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileError(ID, "PROGRAM");
   
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int ShaderProgram::LoadShader(string path, int type)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string shaderCode;
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path); // open files
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();   // read file's buffer contents into streams
        shaderFile.close(); // close file handlers
        shaderCode = shaderStream.str(); // convert stream into string
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = shaderCode.c_str();
    
    unsigned int ID;
    ID = glCreateShader(type==1?GL_VERTEX_SHADER: GL_FRAGMENT_SHADER);
    glShaderSource(ID, 1, &vShaderCode, NULL);
    glCompileShader(ID);
    CheckCompileError(ID, type==1? "VERTEX":"FRAGMENT");

    return ID;
}

void ShaderProgram::CheckCompileError(unsigned int shader, string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
 
void ShaderProgram::Delete()
{
    glDeleteProgram(ID);
}

void ShaderProgram::Use()
{
    glUseProgram(ID);
}

void ShaderProgram::SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    int location = glGetUniformLocation(ID, name);
    glUniform4f(location, v0, v1, v2, v3);
}

void ShaderProgram::SetUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    int location = glGetUniformLocation(ID, name);
    glUniform3f(location, v0, v1, v2);
}

void ShaderProgram::SetUniform(const GLchar* name, GLint v0)
{
    int location = glGetUniformLocation(ID, name);
    glUniform1i(location, v0);
}
