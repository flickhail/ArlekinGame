#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <string>

namespace GameEngine
{
	class Texture
	{
	public:
		Texture(const std::string& imagePath, GLenum format = GL_RGB);

		void Bind(GLenum texUnit = GL_TEXTURE0);

	private:
		unsigned int ID;
	};
}
