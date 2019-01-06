#include "pch.h"
#include "Koch.h"
#include "Tool.h"

#define PI 3.1415926
Koch::Koch(glm::vec3 start, glm::vec3 end, glm::vec3 cameraFront)
{
	allPoints = Fold(start, end, 3);
}


Koch::~Koch()
{
}


std::vector<glm::vec3> Koch::Fold(glm::vec3 a, glm::vec3 b, int level) {
	std::vector<glm::vec3> result;
	glm::vec3 dir = b - a;
	dir /= 3;
	glm::vec3 point1 = a + dir;
	glm::vec3 point3 = a + dir + dir;
	glm::vec3 point2(0);

	float rotate;
	if (b.y >= a.y) {
		rotate = atan((b.y - a.y) / (b.x - a.x));
		if (a.x < b.x) {
			point2.x = point1.x + Length_2D(dir)*cos(rotate + PI / 3);
			point2.y = point1.y + Length_2D(dir)*sin(rotate + PI / 3);
		}
		else
		{
			point2.x = point1.x - Length_2D(dir)*cos(rotate + PI / 3);
			point2.y = point1.y - Length_2D(dir)*sin(rotate + PI / 3);
		}
	}
	else
	{
		rotate = atan((a.y - b.y) / (b.x - a.x));
		if (b.x>a.x) {
			point2.x = point3.x - Length_2D(dir)*cos(rotate + PI / 3);
			point2.y = point3.y + Length_2D(dir)*sin(rotate + PI / 3);
		}
		else
		{
			point2.x = point3.x + Length_2D(dir)*cos(rotate + PI / 3);
			point2.y = point3.y - Length_2D(dir)*sin(rotate + PI / 3);
		}
	}

	

	if (level == 1)
	{
		result.push_back(a);
		result.push_back(point1);
		result.push_back(point2);
		result.push_back(point3);
		result.push_back(b);
	}
	else
	{
		result.push_back(a);
		level--;
		std::vector<glm::vec3> tmp1 = Fold(a, point1, level);
		std::vector<glm::vec3> tmp2 = Fold(point1, point2, level);
		std::vector<glm::vec3> tmp3 = Fold(point2, point3, level);
		std::vector<glm::vec3> tmp4 = Fold(point3, b, level);

		result.insert(result.end(), tmp1.begin(), tmp1.end());
		result.insert(result.end(), tmp2.begin(), tmp2.end());
		result.insert(result.end(), tmp3.begin(), tmp3.end());
		result.insert(result.end(), tmp4.begin(), tmp4.end());
		result.push_back(b);

	}
	return result;

}

void Koch::Draw() {
	if (VAO == 0) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*allPoints.size(), &allPoints[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, allPoints.size());
	glBindVertexArray(0);
}