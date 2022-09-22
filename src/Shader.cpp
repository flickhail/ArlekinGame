#include "Shader.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#include <stdexcept>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	constexpr int infoLogBufSize{ 512 };

	std::string vertexCode;
	std::string fragmentCode;
	

	try
	{
		
		std::ifstream vertexFileStream;
		std::ifstream fragmentFileStream;
		vertexFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		vertexFileStream.open(vertexPath);
		fragmentFileStream.open(fragmentPath);

		std::stringstream vertexStringStream;
		std::stringstream fragmentStringStream;

		vertexStringStream << vertexFileStream.rdbuf();
		fragmentStringStream << fragmentFileStream.rdbuf();

		vertexCode = vertexStringStream.str();
		fragmentCode = fragmentStringStream.str();

		vertexFileStream.close();
		fragmentFileStream.close();
	}
	catch (const std::ios_base::failure&)
	{
		throw std::runtime_error("Shader.Shader error: can't open shader files");
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
		std::string error = std::format("Shader.Shader error: vertex shader has not successful compiled:\n{}", infoLog);
		throw std::runtime_error(error);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, infoLogBufSize, nullptr, infoLog);
		std::string error = std::format("Shader.Shader error: fragment shader has not successful compiled:\n{}", infoLog);
		throw std::runtime_error(error);
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, infoLogBufSize, nullptr, infoLog);
		std::string error = std::format("Shader.Shader error: shader program has not successful compiled:\n{}", infoLog);
		throw std::runtime_error(error);
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
