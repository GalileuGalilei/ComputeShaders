#ifndef SHADER_H
#define SHADER_H


class ShaderProgram
{
public:
	unsigned int ShaderProgramID;
	ShaderProgram();
	void CreateShader(GLenum ShaderType, const char* Path);
	void Use();
	void Delete();
};
#endif