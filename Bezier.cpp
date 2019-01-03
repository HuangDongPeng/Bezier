#include "pch.h"
#include "Bezier.h"


Bezier::Bezier(std::vector<glm::vec3> points,float lerpStep)
{
	for (float lerpArg = 0.00f; lerpArg < 1; lerpArg += lerpStep) {
		glm::vec3 point = GetPoint(points, lerpArg);
		allPoints.push_back(point);
	}
}

Bezier::~Bezier()
{
}

unsigned int bezierVAO = 0, bezierVBO = 0;
void Bezier::DrawCurve() {
	if (bezierVAO == 0) {
		int size =  VectorSizeByte(allPoints);
		glGenVertexArrays(1, &bezierVAO);
		glBindVertexArray(bezierVAO);
		glGenBuffers(1, &bezierVBO);
		glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);
		glBufferData(GL_ARRAY_BUFFER, size, &allPoints[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
	glBindVertexArray(bezierVAO);
	glDrawArrays(GL_LINE_STRIP, 0, allPoints.size());
	glBindVertexArray(0);

}

//只有三个点的曲线生成
glm::vec3 Bezier::CalculatePoint(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, float lerpArg) {
	glm::vec3 vec12 = point2 - point1;
	glm::vec3 vec23 = point3 - point2;

	glm::vec3 point12 = point1 + vec12 * lerpArg;
	glm::vec3 point23 = point2 + vec23 * lerpArg;

	glm::vec3 vecNew = point23 - point12;
	glm::vec3 result = point12 + vecNew * lerpArg;
	return result;
}
glm::vec3 Bezier::GetPointSimple(std::vector<glm::vec3> points, float lerpArg)
{
	return CalculatePoint(points[0], points[1], points[2], lerpArg);
}

//提取前三个点生成中间点
void Bezier::CalcPoint(std::vector<glm::vec3> points, float lerpArg, std::vector<glm::vec3>& tmpVector) {
	if (points.size() < 3) {
		return;
	}

	//最终形成贝塞尔曲线的点
	if (points.size() == 3) {
		tmpVector.push_back(CalculatePoint(points[0], points[1], points[2], lerpArg));
		return;
	}

	//生成的中间点
	glm::vec3 tmpPoint = GetPointSimple(points, lerpArg);
	tmpVector.push_back(tmpPoint);
	points.erase(points.begin());
	CalcPoint(points, lerpArg, tmpVector);
}

//超过三个点递归获取曲线点
glm::vec3 Bezier::GetPoint(std::vector<glm::vec3>& points, float lerpArg) {
	if (points.size() == 3) {
		return GetPointSimple(points, lerpArg);
	}
	if (points.size() > 3) {
		glm::vec3 result(0);
		std::vector<glm::vec3> tmpPoints;
		CalcPoint(points, lerpArg, tmpPoints);
		if (tmpPoints.size() >= 3)
			return GetPoint(tmpPoints, lerpArg);
		else
		{
			result = tmpPoints[0] + lerpArg * (tmpPoints[1] - tmpPoints[0]);
		}
		return result;
	}
	else
	{
		return glm::vec3(0);
	}
}
