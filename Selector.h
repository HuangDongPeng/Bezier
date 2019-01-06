#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Selector
{
public:
	Selector(int screenWidth_IN, int screenHeight_IN, glm::mat4& view_IN, glm::mat4& projection_IN);
	~Selector();

	glm::mat4* projection;
	glm::mat4* view;

	int screenWidth, screenHeight;
	void Select(float screenX, float screenY, std::vector<glm::vec3*>& controlPointsManager, float xoffset, float yoffset);
};

