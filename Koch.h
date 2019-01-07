#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Koch
{
public:
	Koch(std::vector<glm::vec3> points, int level);
	~Koch();

	void Draw();

private:
	unsigned int VAO = 0, VBO = 0;
	std::vector<glm::vec3> allPoints;
	std::vector<glm::vec3> Fold(glm::vec3 a, glm::vec3 b, int level);
};

