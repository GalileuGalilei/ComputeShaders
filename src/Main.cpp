#include <iostream>

#include "glad/glad.h" 
#include "GLFW/glfw3.h"
#include <glm/gtc/type_ptr.hpp>

#include "ShadersPro.h"
#include "Mesh.h"
#include "Camera.h"


int window_width = 520;
int window_heigh = 520;
Camera cam(window_width, window_heigh);
GLFWwindow* window;

#pragma region vertices_vectors

std::vector<float> squarePosition =
{          
	//quadrado que chegue at[e as bordas da tela
	-1.0f, 1.0f, 0.0f,	//0
	1.0f, 1.0f, 0.0f,	//1
	1.0f, -1.0f, 0.0f,	//3
	-1.0f, -1.0f, 0.0f	//2
};

std::vector<float> squareTexture =
{
	0.0f, 1.0f,	//0
	0.0f, 0.0f,	//1
	1.0f, 0.0f,	//2
	1.0f, 1.0f	//3
};

std::vector<GLuint> SquareIndice =
{
	0,1,3, //triangle
	3,2,1  //triangle
};

#pragma endregion

#pragma region Callbacks

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, source = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, source, message);
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cam.OnWindowResize(width, height);
}

void OnMouseInput(GLFWwindow* window, double xpos, double ypos)
{
	cam.OnMouseInput(xpos, ypos);
}

void OnKeyInput(GLFWwindow* window, int, int, int, int)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

#pragma endregion

void InitGladAndGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(window_width, window_heigh, "JANELA", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, OnMouseInput);
	glfwSetKeyCallback(window, OnKeyInput);

	glfwSetFramebufferSizeCallback(window, OnWindowResize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void InitOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//errors
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

	//Viewport
	glViewport(0, 0, window_width, window_heigh);
}

int main()
{
	InitGladAndGLFW();
	InitOpenGL();		

	ShaderProgram SHADER;
	
	SHADER.CreateShader(GL_VERTEX_SHADER, "shaders/VertexShader.glsl");
	SHADER.CreateShader(GL_FRAGMENT_SHADER, "shaders/FragmentShader.glsl");
	SHADER.CreateComputeShader("fractal", "shaders/fractal.glsl");
	SHADER.Use();

	//textures
	RenderTexture* render_tex = new RenderTexture(window_width, window_heigh, GL_RGBA32F);
	SHADER.SetTexture(render_tex, "Texture0");
	SHADER.ActivateTexture(render_tex);
	SHADER.DispatchComputeShader("fractal", window_width, window_heigh, 1);

	//vertices
	Mesh* mesh = new Mesh();
	mesh->AddVerticesAttribute(0, squarePosition, 3);
	mesh->AddVerticesAttribute(1, squareTexture, 2);
	mesh->AddIndices(SquareIndice);
		
	//transformations
	glm::mat4 model_matrix(1.0f);
	int model_location = glGetUniformLocation(SHADER.ShaderProgramID, "model_matrix");

	//Update loop
	while (!glfwWindowShouldClose(window))
	{
		//background_color
		glClearColor(0.75f, 0.23f, 0.46f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera inputs
		cam.OnKeyInput(window);
		cam.SetMatrices(&SHADER);
		
		//binding all
		SHADER.ActivateTexture(render_tex);
		SHADER.Use();

		//models uniforms
		glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));

		//drawing
		mesh->DrawMesh();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	mesh->DeleteMesh();
	SHADER.Delete();
	glfwTerminate();

	return 0;
}




