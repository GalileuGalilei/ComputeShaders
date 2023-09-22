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

void ShaderProgram::CreateComputeShader(const char* Name, const char* Path)
{
	GLuint ComputeProgramID = glCreateProgram();
	computeShaderPrograms[Name] = ComputeProgramID;

	GLuint ShaderT = glCreateShader(GL_COMPUTE_SHADER);
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

	glAttachShader(ComputeProgramID, ShaderT);
	glLinkProgram(ComputeProgramID);

	glGetProgramiv(ComputeProgramID, GL_LINK_STATUS, &sucesso);

	if (!sucesso)
	{
		int logLenght;
		glGetProgramiv(ComputeProgramID, GL_INFO_LOG_LENGTH, &logLenght);
		ErrorInfo = new char[logLenght + 1];

		glGetProgramInfoLog(ShaderProgramID, 512, NULL, ErrorInfo);

		if (logLenght > 0)
		{
			std::cout << "LINK ERROR: \n" << ErrorInfo << std::endl;
		}

		delete[] ErrorInfo;
	}

	glDeleteShader(ShaderT);
}

void ShaderProgram::Use()
{
	glUseProgram(ShaderProgramID);
}

void ShaderProgram::DispatchComputeShader(const char* Name, unsigned int x, unsigned int y, unsigned int z)
{
	glUseProgram(computeShaderPrograms[Name]);
	glDispatchCompute(x, y, z);
}

void ShaderProgram::Delete()
{
	glDeleteProgram(ShaderProgramID);

	for (auto compute : computeShaderPrograms)
	{
		glDeleteProgram(compute.second);
	}
}

void ShaderProgram::SetTexture(Texture* tex, const char* sampler)
{
	tex->Bind();
	GLuint location = glGetUniformLocation(ShaderProgramID, sampler);
	glUniform1i(location, texturesBinds.size());

	for (auto compute : computeShaderPrograms)
	{
		GLuint location = glGetUniformLocation(compute.second, sampler);
		glUniform1i(location, texturesBinds.size());
	}

	texturesBinds.insert(std::pair<unsigned int, unsigned int>(texturesBinds.size(), tex->Id()));
	tex->Unbind();
}

GLuint ShaderProgram::GetUniformLocation(const char* Name)
{
	GLuint location = glGetUniformLocation(ShaderProgramID, Name);
	return location;
}

GLuint ShaderProgram::GetUniformLocation(const char* Name, const char* computeShaderName)
{
	GLuint location = glGetUniformLocation(computeShaderPrograms[computeShaderName], Name);
	return location;
}

void ShaderProgram::ActivateTexture(Texture* tex)
{
	glActiveTexture(texturesBinds[tex->Id()] + GL_TEXTURE0);
	tex->Bind();
}


