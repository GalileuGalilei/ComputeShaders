#include "glad/glad.h" 
#include "GLFW/glfw3.h"
#include "iostream"
#include "stb_image.h"
#include "glfw/include/ShadersPro.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define PI 3.14159265358979
#define MARGINS 0.6


double yaw = -PI * 0.5, pitch; //yaw = 360° e pitch = cima/baixo

int window_width = 720;
int window_heigh = 480;
float lastX = window_width * 0.5f;
float lastY = window_heigh * 0.5f;
bool first_mouse = true;



float line[] =
{
	-1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f
};

float square[] =
{          
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
};

float cube[] = 
{  //vertice:			  texturas:
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cube_position[] =
{
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

GLuint Indice[] =
{
	0,1,2,
	1,2,3
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

void key_input(GLFWwindow* window)
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

void mouse_input(GLFWwindow* window, double xpos, double ypos)
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


int main()
{
	//Glfw and window initialization
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(window_width, window_heigh, "JANELA", NULL, NULL);
		glfwMakeContextCurrent(window);
		glfwSetCursorPosCallback(window, mouse_input);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//GLAD load
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
		}
		glEnable(GL_DEPTH_TEST);

	//Viewport
		glViewport(0, 0, window_width, window_heigh);
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int heigh)->void
		{
			glViewport(0, 0, width, heigh);
			glm::mat4 projection_matrix = glm::perspective((float)PI / 4, (float)width / heigh, 0.1f, 300.0f);
			glm::mat4 aux = glm::mat4(MARGINS);
			projection_matrix *= aux;
		});

		

	//Shaders creation
		ShaderProgram SHADER;
	
		SHADER.CreateShader(GL_VERTEX_SHADER, "shaders/VertexShader.glsl");
		SHADER.CreateShader(GL_FRAGMENT_SHADER, "shaders/FragmentShader.glsl");
	
		SHADER.Use();

	//Texture
		
		//variables
		int image_width, image_heigh, image_chanels;
		unsigned int textureID[1];
		unsigned char* image_data = stbi_load("textures/metal.jpg", &image_width, &image_heigh, &image_chanels, 0);

		//texture id creation
		glGenTextures(1, &textureID[0]);

		//texture bind
		glBindTexture(GL_TEXTURE_2D, textureID[0]);

		//texture parameters
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//texture error test and generation
		if (image_data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_heigh, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Error: texture load failed :(" << std::endl;
		}

		//unbinding / deleting
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image_data);

		//texture to shader
		int TexLocation = glGetUniformLocation(SHADER.ShaderProgramID,"Texture0");
		glUniform1i(TexLocation, 0);

	//tranformations
		//scalar-matrix multiplication, matrix-vector
		//multiplication and matrix-matrix multiplication

		glm::mat4 tran = glm::mat4(1.0f);
		tran = glm::rotate(tran, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int mat_location = glGetUniformLocation(SHADER.ShaderProgramID, "transform");
		glUniformMatrix4fv(mat_location, 1, GL_FALSE, glm::value_ptr(tran));


		//going 3D...

		//model...
		glm::mat4 model_matrix(1.0f);

		//view...
		glm::mat4 view_matrix(1.0f);
		view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));

		//perpective projection...
		glm::mat4 projection_matrix = glm::perspective((float)PI / 4, (float)window_width / window_heigh, 0.1f, 300.0f);


	//Vertice buffer generation 
		//GLuint EBO; sem indices po enquanto...
		GLuint VBO[2];
		GLuint VAO[2];


		//buffer creation
		glGenBuffers(2, VBO);
		//glGenBuffers(1, &EBO); //indices q por enquanto vao fica de lado
		glGenVertexArrays(2, VAO);

		//buffer binding
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//buffer data assingment
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Indice), Indice, GL_STATIC_DRAW);

		//vertex attributes
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float), (void*)0);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//buffer unbinding
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

		//---------------------

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	//Update loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}

		key_input(window);
		


		//background_color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.75f, 0.23f, 0.46f, 1.0f);

		//binding all
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[0]);
		glBindVertexArray(VAO[0]);
		SHADER.Use();

		//updating

		view_matrix = glm::lookAt
		(
			camera_position,
			camera_target,
			glm::vec3(0.0f,1.0f,0.0f)
		);


		int model_location = glGetUniformLocation(SHADER.ShaderProgramID, "model");
		int view_location = glGetUniformLocation(SHADER.ShaderProgramID, "view");
		int projection_location = glGetUniformLocation(SHADER.ShaderProgramID, "projection");
		

		glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(view_location, 1, false, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_location, 1, false, glm::value_ptr(projection_matrix));


		//draw and buffer_Swap
		for (int i = 0; i < 5; i++)
		{
			model_matrix = glm::translate(glm::mat4(1.0f), cube_position[i]);
			model_matrix = glm::rotate(model_matrix, (float)glfwGetTime() * ((float)PI / 4), glm::vec3(1.0f, 0.5f, 0.5f));
			glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(VAO[1]);
		
		for(int i = 1; i < 150; i++)
		{
			model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -5.0f - camera_position.y, 75 - i - mod(1, camera_position.z)));
			model_matrix = glm::translate(model_matrix, camera_position);
			model_matrix = glm::scale(model_matrix, glm::vec3(75.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));
			glDrawArrays(GL_LINES, 0, 2);
		}
		
		for (int i = 1; i < 150; i++)
		{
			model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(75 - i - mod(1, camera_position.x), -5.0f - camera_position.y, 0.0f));
			model_matrix = glm::translate(model_matrix, camera_position);
			model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 75.0f));
			model_matrix = glm::rotate(model_matrix, (float)PI / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));
			glDrawArrays(GL_LINES, 0, 2);
		}


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	//Unbinding / deleting all
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(2, VBO);
		//glDeleteBuffers(1, &EBO);
		SHADER.Delete();
		glfwTerminate();

	return 0;
}




