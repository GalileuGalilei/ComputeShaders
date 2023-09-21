#include "glad/glad.h" 
#include "GLFW/glfw3.h"
#include "iostream"
#include "ShadersPro.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "glm/gtc/type_ptr.hpp"
#define PI 3.14159265358979
#define MARGINS 0.6

int window_width = 512;
int window_heigh = 512;
float lastX = window_width * 0.5f;
float lastY = window_heigh * 0.5f;
bool first_mouse = true;
GLFWwindow* window;


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

glm::vec3 camera_position(0.0f, 0.0f, 3.0f);
glm::vec3 camera_target(0.0f, 0.0f, -1.0f);

glm::vec3 camera_direction = glm::normalize(camera_position - camera_target); //sentido inverso
glm::vec3 camera_x_axis = glm::cross(camera_direction, glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 camera_y_axis = glm::cross(camera_direction, camera_x_axis);

float mod(float a, float b)
{

	if (abs(a) > abs(b))
	{
		return b;
	}

	int aux = (int)b / a;
	b -= aux * a;

	return b;
}

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
	glm::mat4 projection_matrix = glm::perspective((float)PI / 4, (float)width / height, 0.1f, 300.0f);
	glm::mat4 aux = glm::mat4(MARGINS);
	projection_matrix *= aux;
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
	//glfwSetCursorPosCallback(window, OnMouseInput);
	glfwSetFramebufferSizeCallback(window, OnWindowResize);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void InitOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

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

	//Shaders creation
		ShaderProgram SHADER;
	
		SHADER.CreateShader(GL_VERTEX_SHADER, "shaders/VertexShader.glsl");
		SHADER.CreateShader(GL_FRAGMENT_SHADER, "shaders/FragmentShader.glsl");
		SHADER.CreateComputeShader("displace", "shaders/displace.glsl");
		SHADER.Use();

		//texture to shader
		Texture* tex = new Texture("metal.jpg", false);
		SHADER.SetTexture(tex, "Texture0");

		Mesh* mesh = new Mesh();
		mesh->AddVerticesAttribute(0, squarePosition, 3);
		mesh->AddVerticesAttribute(1, squareTexture, 2);
		mesh->AddIndices(SquareIndice);
		
		//transformations
		glm::mat4 model_matrix(1.0f);
		int model_location = glGetUniformLocation(SHADER.ShaderProgramID, "model");



	//Update loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}

		//background_color
		glClearColor(0.75f, 0.23f, 0.46f, 1.0f);

		//binding all
		SHADER.ActivateTexture(tex);
		SHADER.DispatchComputeShader("displace", tex->width, tex->height, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		GLenum err = glGetError();

		//printa o erro


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SHADER.Use();


		//updating
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




