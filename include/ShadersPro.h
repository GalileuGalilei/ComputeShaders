#ifndef SHADER_H
#define SHADER_H
#include <map>
#include "Texture.h"
#include <string>
#include <glad/glad.h>

class ShaderProgram
{
private:
	std::map<unsigned int, unsigned int> texturesBinds;
	std::map<std::string, unsigned int> computeShaderPrograms;

public:
	unsigned int ShaderProgramID;

	ShaderProgram();
	void CreateShader(GLenum ShaderType, const char* Path);
	void CreateComputeShader(const char* Name, const char* Path);

	void SetTexture(Texture* tex, const char* sampler);
	void ActivateTexture(Texture* tex);
	GLuint GetUniformLocation(const char* name);
	GLuint GetUniformLocation(const char* name, const char* computeShaderName);
	
	void Use();
	void DispatchComputeShader(const char* Name, unsigned int NumGroupsX​, unsigned int NumGroupsY, unsigned int NumGroupsZ);
	void Delete();
};
#endif