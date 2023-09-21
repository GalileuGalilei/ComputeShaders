#ifndef SHADER_H
#define SHADER_H
#include <map>
#include "Texture.h"

class ShaderProgram
{
private:
	std::map<unsigned int, unsigned int> texturesBinds;
	std::map<std::string, unsigned int> computeShaderKernerls;

public:
	unsigned int ShaderProgramID;

	ShaderProgram();
	void CreateShader(GLenum ShaderType, const char* Path);
	void CreateComputeShader(const char* Name, const char* Path);

	void SetTexture(Texture* tex, const char* sampler);
	void ActivateTexture(Texture* tex);
	
	void Use();
	void DispatchComputeShader(const char* Name, unsigned int NumGroupsX​, unsigned int NumGroupsY, unsigned int NumGroupsZ);
	void Delete();
};
#endif