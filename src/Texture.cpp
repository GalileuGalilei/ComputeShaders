#include <iostream>
#include <fstream>
#include "Texture.h"
#include "stb_image.h"
#include "glad/glad.h" 

Texture::Texture(const char* filename, bool mipmap)
{
	int image_width, image_heigh, image_chanels;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_heigh, &image_chanels, 0);

	if (image_data == nullptr)
	{
		std::cout << "Failed to load texture: " << filename << std::endl;
		return;
	}

	//image_data = nullptr;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	//default parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image_width, image_heigh, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image_width, image_heigh, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, textureId);
	
	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image_data);

	width = image_width;
	height = image_heigh;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

void Texture::Bind()
{
	glBindImageTexture(0, textureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

int Texture::Id()
{
	return textureId;
}

