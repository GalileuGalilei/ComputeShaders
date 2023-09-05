#ifndef SHADER_H
#define SHADER_H
#include <map>
#include "Texture.h"

class ShaderProgram
{
private:
	std::map<unsigned int, unsigned int> texturesBinds;

public:
	unsigned int ShaderProgramID;
	ShaderProgram();
	void CreateShader(GLenum ShaderType, const char* Path);

	void SetTexture(Texture* tex, const char* sampler);
	void ActivateTexture(Texture* tex);
	
	void Use();
	void Delete();
	
};
#endif