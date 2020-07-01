#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	unsigned int vertexShader, fragmentShader;
	readAndCompileShader(GL_VERTEX_SHADER, vertexShader, vertexShaderPath);
	readAndCompileShader(GL_FRAGMENT_SHADER, fragmentShader, fragmentShaderPath);

	createProgram(Id, vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::readAndCompileShader(GLenum shaderType, unsigned int& shader, const char* shaderFilePath)
{
	std::string shaderCodeStr;

	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		shaderFile.open(shaderFilePath);

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		shaderCodeStr = shaderStream.str();
	}
	catch (std::ifstream::failure error)
	{
		printf("ERROR: Failed to read shader file at: %s\n", shaderFilePath);
	}

	const char* shaderCode = shaderCodeStr.c_str();

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR: Failed to compile shader at: %s\n", shaderFilePath);
		printf("INFO_LOG: %s\n", infoLog);
	}
	else
	{
		printf("SUCCESS: Compiled shader at: %s\n", shaderFilePath);
	}
}

void Shader::createProgram(unsigned int& shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
{
	shaderProgram = glCreateProgram();

	if (!vertexShader)
	{
		printf("ERROR: No vertex shader provided\n");
	}
	if (!fragmentShader)
	{
		printf("ERROR: No fragment shader provided\n");
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR: Failed to link ShaderProgram!\n");
		printf("INFO_LOG: %s\n", infoLog);
	}
	else
	{
		printf("ShaderProgram linked successfully\n");
	}
}

void Shader::setBool(const char* attributeName, bool value)
{
	int location = glGetUniformLocation(Id, attributeName);
	glUniform1i(location, (int)value);
}

void Shader::setInt(const char* attributeName, int value)
{
	int location = glGetUniformLocation(Id, attributeName);
	glUniform1i(location, value);
}

void Shader::setFloat(const char* attributeName, float value)
{
	int location = glGetUniformLocation(Id, attributeName);
	glUniform1f(location, value);
}