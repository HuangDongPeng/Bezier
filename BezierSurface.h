#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "BezierCurver.h"
class BezierSurface
{
public:
	BezierSurface(std::vector<BezierCurver*> baseCurvers);
	~BezierSurface();

	std::vector<BezierCurver*> baseCurvers;
	std::vector<BezierCurver> allCurvers;//基础曲线

	std::vector<glm::vec3> allPoints;//所有控制点
	std::vector<glm::vec3> allNormals;
	std::vector<glm::vec2> allTexCoords;
	void DrawCurversControlPoints();
	void DrawSurface();
	void DrawCurvers();
	void DrawPoints();


	void InitSurface();
	int VectorSizeByte(std::vector<glm::vec3> vector);

private :
	unsigned int surfaceVAO = 0, surfaceVBO = 0, surfaceEBO = 0,EBOCount=0;
	float lerpStep;
};

