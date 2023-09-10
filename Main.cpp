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


double yaw = -PI * 0.5, pitch; //yaw = 360° e pitch = cima/baixo

int window_width = 720;
int window_heigh = 480;
float lastX = window_width * 0.5f;
float lastY = window_heigh * 0.5f;
bool first_mouse = true;
GLFWwindow* window;


std::vector<float> squarePosition =
{          
	-0.5f, 0.5f, 0.0f,	//0
	-0.5f, -0.5f, 0.0f, //1
	0.5f, -0.5f, 0.0f,	//2
	0.5f, 0.5f, 0.0f	//3
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
	0,1,2, //triangle
	1,2,3  //triangle
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

void OnKeyboardInput(GLFWwindow* window)
{
	
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera_position += camera_direction * 0.5f;
		camera_target += camera_direction * 0.5f;
	}
	
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera_position -= camera_direction * 0.5f;
		camera_target -= camera_direction * 0.5f;
	}

	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera_position -= camera_x_axis * 0.5f;
		camera_target -= camera_x_axis * 0.5f;
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera_position += camera_x_axis * 0.5f;
		camera_target += camera_x_axis * 0.5f;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		camera_position.y += 0.5f;
		camera_target.y += 0.5f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		camera_position.y -= 0.5f;
		camera_target.y -= 0.5f;
	}

	camera_direction = glm::normalize(camera_target - camera_position); 
	camera_x_axis = glm::cross(camera_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	camera_y_axis = glm::cross(camera_direction, camera_x_axis);
}

void OnMouseInput(GLFWwindow* window, double xpos, double ypos)
{
	if (first_mouse)
	{
		first_mouse = false;
		xpos = lastX;
		ypos = lastY;
	}

	float offsetX = xpos - lastX;
	float offsetY = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	yaw += offsetX * 0.002;
	pitch += offsetY * 0.002;

	if (pitch > PI*0.4f)
	{
		pitch = PI * 0.4f;
	}
	if (pitch < -PI * 0.4f)
	{
		pitch = -PI * 0.4f;
	}

	camera_target.x = cos(yaw)*cos(pitch);
	camera_target.y = sin(pitch);
	camera_target.z = sin(yaw) * cos(pitch);
	camera_target += camera_position;
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
	glfwSetCursorPosCallback(window, OnMouseInput);
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
		SHADER.Use();

		//texture to shader
		Texture* tex = new Texture("metal.jpg", true);
		SHADER.SetTexture(tex, "Texture0");

		Mesh* mesh = new Mesh();
		mesh->AddVerticesAttribute(0, squarePosition, 3);
		mesh->AddVerticesAttribute(1, squareTexture, 2);
		mesh->AddIndices(SquareIndice);
		
		//transformations
		glm::mat4 model_matrix(1.0f);
		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection_matrix = glm::perspective((float)PI / 4, (float)window_width / window_heigh, 0.1f, 300.0f);

		int model_location = glGetUniformLocation(SHADER.ShaderProgramID, "model");
		int view_location = glGetUniformLocation(SHADER.ShaderProgramID, "view");
		int projection_location = glGetUniformLocation(SHADER.ShaderProgramID, "projection");


	//Update loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}

		OnKeyboardInput(window);
		


		//background_color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.75f, 0.23f, 0.46f, 1.0f);

		//binding all
		SHADER.ActivateTexture(tex);
		SHADER.Use();

		//updating

		view_matrix = glm::lookAt
		(
			camera_position,
			camera_target,
			glm::vec3(0.0f,1.0f,0.0f)
		);
		

		glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(view_location, 1, false, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_location, 1, false, glm::value_ptr(projection_matrix));


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




