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

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	//default parameters
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_heigh, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	
	if (mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image_data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::Id()
{
	return textureId;
}

