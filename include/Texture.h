#pragma once

/// <summary>
/// Texture that can be read by shaders
/// </summary>
class Texture
{
protected:
	unsigned int textureId;
	unsigned int format;
	int width = 0;
	int height = 0;

public:
	Texture(const char* filename, bool mipmap, unsigned int format);
	~Texture();

	virtual void Bind();
	virtual void Unbind();
	
	//properties
	int Width();
	int Height();
	int Id();
	unsigned int Format();
};

/// <summary>
/// Black texture that can be read and written by shaders
/// </summary>
/// <remarks>Unlike normal textures, RenderTexture do not have mipmaps</remarks>
class RenderTexture : public Texture
{	
public:
	RenderTexture(int width, int height, unsigned int format);

	void* GetPixels();
	void SaveToFile(const char* filename);
	
	void Bind() override;
	void Unbind() override;
};