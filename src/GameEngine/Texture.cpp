#include "Texture.hpp"

#include <iostream>
#include <stdexcept>
#include <format>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace GameEngine;

Texture::Texture(const std::string& imagePath, GLenum format)
{
	int width;
	int height;
	int colorChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &colorChannels, 0);
	if (!data)
	{
		stbi_image_free(data);
		std::string error = std::format("Texture.Texture error: cannot load image file:\n{}", imagePath);
		throw std::runtime_error(error);
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::Bind(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, ID);
}