#pragma once
#include "glad/glad.h" 
#include "GLFW/glfw3.h"
#include <vector>
#include <map>

class Mesh
{
private:
	std::map<int, std::vector<float>> vertices;
	std::map<int, int> attributesOffset;
	std::map<int, int> attributesSize;
	std::vector<unsigned int> indices;
	
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;
	int stride = 0;
	int attributes = 0;

	void CreateBuffers();
	float* GetBufferData(int& size);
	
public:

	/// <param name="index">attribute number that should be activated</param>
	/// <param name="vertices">the normalized data itself</param>
	/// <param name="size">number os floats of the attribute. For example, vec3 = 3 floats </param>
	void AddVerticesAttribute(int index, std::vector<float> vertices, int size);
	void AddIndices(std::vector<unsigned int> indices);
	void Bind();
	void Unbind();

	void DrawMesh();
	void DeleteMesh();
};

