#pragma once

#include <glad/glad.h>

class Shader
{
private:
	void readAndCompileShader(GLenum shaderType, unsigned int& shader, const char* shaderFilePath);
	void createProgram(unsigned int& shaderProrgamId, unsigned int vertexShader, unsigned int fragmentShader);

public:
	unsigned int Id;
	
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void setBool(const char* attributeName, bool value);
	void setInt(const char* attributeName, int value);
	void setFloat(const char* attributeName, float value);
};