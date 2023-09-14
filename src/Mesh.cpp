#include "Mesh.h"
#include <iostream>

void Mesh::AddVerticesAttribute(int index, std::vector<float> data, int count)
{
	if (vertices.size() != data.size() / count && vertices.size() != 0)
	{
		std::cout << "ERROR: each vertex attribute must be equal to the number of vertices" << std::endl;
		return;
	}

	for (int i = 0; i < data.size(); i++)
	{
		vertices[i / count].push_back(data[i]);
	}

	attributesOffset[index] = stride;
	attributesSize[index] = count;
	stride += count;
	attributes++;
}

void Mesh::AddIndices(std::vector<unsigned int> indices)
{
	this->indices = indices;
}

void Mesh::Bind()
{
	if (VBO == 0)
	{
		CreateBuffers();
	}

	glBindVertexArray(VAO);
}

void Mesh::Unbind()
{
	glBindVertexArray(0);
}

float* Mesh::GetBufferData(int& size)
{
	int vertexSize = vertices[0].size();
	int numVertices = vertices.size();
	float* verticesData = new float[vertexSize * numVertices];
	size = 0;
	
	for (int i = 0; i < numVertices; i++)
	{
		for (int j = 0; j < vertexSize; j++)
		{
			verticesData[size] = vertices[i][j];
			size++;
		}
	}

	return verticesData;
}

void Mesh::CreateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int size;
	float* data = GetBufferData(size);
	
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
	
	for (int i = 0; i < attributes; i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributesSize[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(attributesOffset[i] * sizeof(float)));
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::DeleteMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	vertices.clear();
	indices.clear();
	attributesOffset.clear();
}

void Mesh::DrawMesh()
{
	Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}