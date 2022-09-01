#include "Shader.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::ifstream vertexFStream;
	std::ifstream fragmentFStream;
	std::string vertexCode;
	std::string fragmentCode;
	constexpr int infoLogBufSize{ 512 };

	vertexFStream.exceptions	(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFStream.exceptions	(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexFStream.open(vertexPath);
		fragmentFStream.open(fragmentPath);

		std::stringstream vertexStream;
		std::stringstream fragmentStream;

		vertexStream << vertexFStream.rdbuf();
		fragmentStream << fragmentFStream.rdbuf();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		vertexFStream.close();
		fragmentFStream.close();
	}
	catch (std::ifstream::failure)
	{
		std::cout << "Error: can't open shader files" << std::endl;
	}

	const char* vertShaderCode = vertexCode.c_str();
	const char* fragShaderCode = fragmentCode.c_str();

	unsigned int vertex{};
	unsigned int fragment{};
	int success{};
	char infoLog[infoLogBufSize]{};

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, infoLogBufSize, nullptr, infoLog);
		std::cout << "Error: vertex shader have not successful compiled:\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, infoLogBufSize, nullptr, infoLog);
		std::cout << "Error: fragment shader have not successful compiled:\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, infoLogBufSize, nullptr, infoLog);
		std::cout << "Error: shader program have not successful compiled:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat4(const std::string& name, float val1, float val2, float val3, float val4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), val1, val2, val3, val4);
}

void Shader::SetMat4f(const std::string& name, const float* matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, matrix);
}

unsigned int Shader::GetID() const
{
	return ID;
}
