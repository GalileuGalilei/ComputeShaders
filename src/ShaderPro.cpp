#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include <iostream>
#include "ShadersPro.h"


//shaders
char** ShaderSource(const char* Path, GLint& lenght)
{
	std::ifstream FileSource(Path);
	if (FileSource.is_open())
	{
		FileSource.seekg(0, FileSource.end);
		lenght = FileSource.tellg();
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
	GLint lenght;
	char** source = ShaderSource(Path, lenght);
	glShaderSource(ShaderT, 1, source, &lenght);
	glCompileShader(ShaderT);

	int sucesso;
	char* ErrorInfo;
	glGetShaderiv(ShaderT, GL_COMPILE_STATUS, &sucesso);

	if (!sucesso)
	{
		int logLenght;
		glGetShaderiv(ShaderT, GL_INFO_LOG_LENGTH, &logLenght);
		ErrorInfo = new char[logLenght + 1];

		glGetShaderInfoLog(ShaderT, logLenght, NULL, ErrorInfo);

		if (logLenght > 0)
		{
			std::cout << "GLSL ERROR: \n" << ErrorInfo << std::endl;
		}

		delete[] ErrorInfo;
	}

	glAttachShader(ShaderProgramID, ShaderT);
	glLinkProgram(ShaderProgramID);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &sucesso);

	if (!sucesso) 
	{
		int logLenght;
		glGetProgramiv(ShaderProgramID, GL_INFO_LOG_LENGTH, &logLenght);
		ErrorInfo = new char[logLenght + 1];

		glGetProgramInfoLog(ShaderProgramID, 512, NULL, ErrorInfo);

		if (logLenght > 0)
		{
			std::cout << "LINK ERROR: \n" << ErrorInfo << std::endl;
		}

		delete[] ErrorInfo;
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

void ShaderProgram::SetTexture(Texture* tex, const char* sampler)
{
	tex->Bind();
	glUniform1i(glGetUniformLocation(ShaderProgramID, sampler), texturesBinds.size());
	texturesBinds.insert(std::pair<unsigned int, unsigned int>(texturesBinds.size(), tex->Id()));
	tex->Unbind();
}

void ShaderProgram::ActivateTexture(Texture* tex)
{
	tex->Bind();
	glActiveTexture(texturesBinds[tex->Id()]);
}


