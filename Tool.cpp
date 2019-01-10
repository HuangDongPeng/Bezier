#include "pch.h"
#include "Tool.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "stb_image.h"
#include <math.h>

std::vector<glm::vec3*> Manager::controlPointsManager;

int VectorSizeByte(std::vector<glm::vec3> vector)
{
	return sizeof(float)*vector.size() * 3;
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

float Length(glm::vec2 a, glm::vec2 b) {
	glm::vec2 tmp = a - b;
	return sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
}

float Length(glm::vec2 dir) {
	return (sqrt(dir.x*dir.x + dir.y*dir.y));
}

float Length_2D(glm::vec3 dir) {
	return (sqrt(dir.x*dir.x + dir.y*dir.y));
	glm::dot(dir, dir);
}

void CombineVector(std::vector<glm::vec3>& a, std::vector<glm::vec3> b) {
	a.insert(a.begin(), b.begin(), b.end());
}

void SetVertiesArr(unsigned int &VAO, unsigned int &VBO, std::vector <glm::vec3*> &points)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) * sizeof(glm::vec3), &(*points[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void SetVertiesArrWithEBO(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, std::vector <glm::vec3*> &VBOpoints, std::vector <glm::vec3> &EBOpoints)
{
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*EBOpoints.size(), &EBOpoints[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VBOpoints.size() * sizeof(glm::vec3), &(*VBOpoints[0]), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
