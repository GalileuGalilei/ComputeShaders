#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <fstream>
#include "Texture.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "glad/glad.h" 

Texture::Texture(const char* filename, bool mipmap, unsigned int format)
{
	int image_chanels;
	unsigned char* image_data = nullptr;
	
	this->format = format;

	if (filename != nullptr)
	{
		image_data = stbi_load(filename, &width, &height, &image_chanels, 0);
	}

	if (filename != nullptr && image_data == nullptr)
	{
		std::cout << "Failed to load texture: " << filename << std::endl;
		return;
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	//default parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
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

int Texture::Width()
{
	return width;
}

int Texture::Height()
{
	return height;
}

unsigned int Texture::Format()
{
	return format;
}

RenderTexture::RenderTexture(int width, int height, unsigned int format) : Texture(nullptr, false, format)
{
	this->width = width;
	this->height = height;

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void RenderTexture::Bind()
{
	glBindImageTexture(0, textureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void RenderTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, format);
}

void* RenderTexture::GetPixels()
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	void* pixels = malloc(width * height * 3);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	return pixels;
}

void RenderTexture::SaveToFile(const char* filename)
{
	char* data = (char*)GetPixels();
	stbi_write_png(filename, width, height, 3, data, width * 3);
}