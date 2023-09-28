#pragma once
#include <glm/glm.hpp>
#include "ShadersPro.h"

class Camera
{
private:
	glm::vec3 camera_position;
	glm::vec3 camera_target;
	glm::vec3 camera_direction;
	glm::vec3 camera_x_axis;
	glm::vec3 camera_y_axis;
	glm::mat4 projection_matrix;

	bool first_mouse = true;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float sensitivity = 0.05f;

	int screen_width = 800;
	int screen_height = 600;
	
public:
	Camera(int width, int height);
	~Camera();
	void OnMouseInput(double xpos, double ypos);
	void OnWindowResize(int width, int height);
	void SetMatrices(ShaderProgram* shader);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraTarget();
	glm::vec3 GetCameraDirection();
	glm::vec3 GetCameraXAxis();
	glm::vec3 GetCameraYAxis();
};

