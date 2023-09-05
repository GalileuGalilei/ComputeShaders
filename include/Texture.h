#pragma once
class Texture
{
private:
	unsigned int textureId;

public:
	Texture(const char* filename, bool mipmap);
	~Texture();

	void Bind();
	void Unbind();
	int Id();
};

