#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <iostream>
#include "ShadersPro.h"


//shaders
char** ShaderSource(const char* Path)
{
	std::ifstream FileSource(Path);
	if (FileSource.is_open())
	{
		FileSource.seekg(0, FileSource.end);
		long long lenght = FileSource.tellg();
		FileSource.seekg(0, FileSource.beg);

		char* CharFile = new char[lenght + 1]{ NULL };

		FileSource.read(CharFile, lenght);

		FileSource.close();

		return &CharFile;
	}
	else
	{
		std::cout << "erro ao abrir arquivo :(";
	}

};


	
ShaderProgram::ShaderProgram()
{
	ShaderProgramID = glCreateProgram();
}

void ShaderProgram::CreateShader(GLenum ShaderType, const char* Path)
{
	GLuint ShaderT = glCreateShader(ShaderType);
	glShaderSource(ShaderT, 1, ShaderSource(Path), NULL);
	glCompileShader(ShaderT);

	int sucesso;
	char ErrorInfo[512];
	glGetShaderiv(ShaderT, GL_COMPILE_STATUS, &sucesso);

	if (!sucesso)
	{
		glGetProgramInfoLog(ShaderT, 512, NULL, ErrorInfo);
		std::cout << "ERRO NO GLSL: \n" << ErrorInfo << std::endl;

	}

	glAttachShader(ShaderProgramID, ShaderT);
	glLinkProgram(ShaderProgramID);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &sucesso);

	if (!sucesso) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, ErrorInfo);
		std::cout << "ERRO DE LINK: \n" << ErrorInfo << std::endl;

	}

	glDeleteShader(ShaderT);



};

void ShaderProgram::Use()
{
	glUseProgram(ShaderProgramID);
}

void ShaderProgram::Delete()
{
	glDeleteProgram(ShaderProgramID);
}


