#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Tool.h"
class BezierCurver
{
public:
	BezierCurver(std::vector<glm::vec3> points, float step);
	~BezierCurver();
	
	float lerpStep = 1;

	std::vector<glm::vec3> allPoints;//线上的点
	std::vector<glm::vec3> controlPoints;//控制点

	void DrawCurve();
	void DrawControlPoints();

private:

	unsigned int bezierVAO = 0, bezierVBO = 0;
	unsigned int controlPointsVAO = 0, controlPointsVBO = 0;

	glm::vec3 CalculatePoint(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, float lerpArg);
	glm::vec3 GetPoint(std::vector<glm::vec3>& points, float lerpArg);
	glm::vec3 GetPointSimple(std::vector<glm::vec3> points, float lerpArg);
	int VectorSizeByte(std::vector<glm::vec3> vector);

	void CalcPoint(std::vector<glm::vec3> points, float lerpArg, std::vector<glm::vec3>& tmpVector);

};


