#include "Camera.h"
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void Camera::RecalculateAxis()
{
	camera_direction = glm::normalize(camera_position - camera_target);
	camera_x_axis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_direction));
	camera_y_axis = glm::cross(camera_direction, camera_x_axis);
}

Camera::Camera(int width, int height)
{
	this->screen_height = height;
	this->screen_width = width;

	camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
	camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	RecalculateAxis();

	OnWindowResize(width, height);
}

Camera::~Camera()
{
}

void Camera::OnKeyInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	float deltaTime = glfwGetTime() - lastUpdate;
	lastUpdate = glfwGetTime();
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_position -= CameraSpeed * camera_direction * deltaTime;
		camera_target -= CameraSpeed * camera_direction * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_position += CameraSpeed * camera_direction * deltaTime;
		camera_target += CameraSpeed * camera_direction * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_position -= camera_x_axis * CameraSpeed * deltaTime;
		camera_target -= camera_x_axis * CameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_position += camera_x_axis * CameraSpeed * deltaTime;
		camera_target += camera_x_axis * CameraSpeed * deltaTime;
	}
	
	RecalculateAxis();
}

void Camera::OnMouseInput(double xpos, double ypos)
{
	if (first_mouse)
	{
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::mat4 rotation_matrix = glm::mat4(1.0f);
	rotation_matrix = glm::translate(rotation_matrix, camera_position);
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(xoffset), camera_y_axis);
	rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yoffset), camera_x_axis);
	rotation_matrix = glm::translate(rotation_matrix, -camera_position);
	camera_target = rotation_matrix * glm::vec4(camera_target, 1.0f);
	
	RecalculateAxis();
}

void Camera::SetMatrices(ShaderProgram* shader)
{
	unsigned int projection_matrix_location = shader->GetUniformLocation("projection_matrix");
	unsigned int view_matrix_location = shader->GetUniformLocation("view_matrix");

	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, glm::value_ptr(GetViewMatrix()));
}

void Camera::OnWindowResize(int width, int height)
{
	projection_matrix = glm::perspective((float)(3.141529 / 4.0), (float)(width / height), 0.1f, 300.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(camera_position, camera_target, camera_y_axis);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return projection_matrix;
}

glm::vec3 Camera::GetCameraPosition()
{
	return camera_position;
}

glm::vec3 Camera::GetCameraTarget()
{
	return camera_target;
}

glm::vec3 Camera::GetCameraDirection()
{
	return camera_direction;
}

glm::vec3 Camera::GetCameraXAxis()
{
	return camera_x_axis;
}

glm::vec3 Camera::GetCameraYAxis()
{
	return camera_y_axis;
}

