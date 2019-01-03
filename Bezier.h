#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Bezier
{
public:
	Bezier();
	~Bezier();


	glm::vec3 CalculatePoint(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, float lerpArg);
	glm::vec3 GetPoint(std::vector<glm::vec3>& points, float lerpArg);
	glm::vec3 GetPointSimple(std::vector<glm::vec3> points, float lerpArg);

	void CalcPoint(std::vector<glm::vec3> points, float lerpArg, std::vector<glm::vec3>& tmpVector);
	void DrawLine(std::vector<glm::vec3> points);

	int VectorSizeByte(std::vector<glm::vec3> vector) {
		return sizeof(float)*vector.size() * 3;
	}
};

