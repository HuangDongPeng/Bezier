#include "pch.h"
#include "BezierSurface.h"

BezierSurface::BezierSurface(std::vector<BezierCurver> baseCurvers)
{
	lerpStep = baseCurvers[0].lerpStep;
	this->baseCurvers = baseCurvers;
	InitSurface();
}


BezierSurface::~BezierSurface()
{
}

void BezierSurface::InitSurface() {

	for (int i = 0; i < baseCurvers[0].allPoints.size(); i++) {
		std::vector<glm::vec3> tmpPoints;
		for (int curverIndex = 0; curverIndex < baseCurvers.size(); ++curverIndex) {
			tmpPoints.push_back(baseCurvers[curverIndex].allPoints[i]);
		}

		BezierCurver newBezierCurver(tmpPoints, lerpStep);
		allCurvers.push_back(newBezierCurver);
		allPoints.insert(allPoints.end(), newBezierCurver.allPoints.begin(), newBezierCurver.allPoints.end());
	}

	int pointCountEveryCow = 1 / lerpStep + 1;
	float texStep = 1.0f / pointCountEveryCow*1.0f;
	int cow = 0;
	float s, t;
	for (int i = 0; i < pointCountEveryCow; i++) {
		int index = cow * pointCountEveryCow;
		for (int j = 0; j < pointCountEveryCow; j++) 
		{
			s = i * texStep;
			t = j * texStep;

			glm::vec2 texCoords(s, t);
			allTexCoords.push_back(texCoords);
		}
		cow++;
	}

}

void BezierSurface::DrawSurface() {
	if (surfaceVAO == 0) {
		//EBO
		int pointCountEveryCow = 1 / lerpStep+1;
		std::vector<unsigned int> eboVector;
		int cow = 0;
		for (int i = 0; i < pointCountEveryCow-1 ; i++) {
			int index = cow * pointCountEveryCow;
			for (int j = 0; j < pointCountEveryCow-1; j++) {
				//设置EBO
				eboVector.push_back(index);
				eboVector.push_back(index + 1);
				eboVector.push_back(index + pointCountEveryCow);

				eboVector.push_back(index + pointCountEveryCow + 1);
				eboVector.push_back(index + pointCountEveryCow);
				eboVector.push_back(index + 1);


				//设置法线(不包括最后一列)
				glm::vec3 normal(0);
				glm::vec3 dir1(0);
				glm::vec3 dir2(0);
				if (index < pointCountEveryCow-1) {
					dir1 = allPoints[index + pointCountEveryCow] - allPoints[index];
					dir2 = allPoints[index + 1] - allPoints[index];
				}
				else 
				{
					dir1 = allPoints[index - 1] - allPoints[index];
					dir2 = allPoints[index + pointCountEveryCow] - allPoints[index];
				}
				normal = glm::normalize(glm::cross(dir1, dir2));
				allNormals.push_back(normal);

				index++;
			}
			cow++;
		}
		//补齐最后一列法线
		glm::vec3 normal(0);
		glm::vec3 dir1(0);
		glm::vec3 dir2(0);
		for (int i = 0; i < pointCountEveryCow; i++) {
			int index = pointCountEveryCow * (pointCountEveryCow - 1);
			if (i < pointCountEveryCow - 1) {
				dir1 = allPoints[index + 1] - allPoints[index];
				dir2 = allPoints[index - pointCountEveryCow] - allPoints[index];
			}
			else
			{
				dir1 = allPoints[index - pointCountEveryCow] - allPoints[index];
				dir2 = allPoints[index - 1] - allPoints[index];
			}
			index++;
			normal = glm::normalize(glm::cross(dir1, dir2));
			allNormals.push_back(normal);
		}

		EBOCount = eboVector.size();

		int size = VectorSizeByte(allPoints);
		int pointsSize = allPoints.size() * sizeof(float) * 3;
		int texCoordSize = allTexCoords.size() * sizeof(float) * 2;
		int test1 = sizeof(glm::vec2);
		int test2 = sizeof(glm::vec3);
		int test3 = sizeof(float);

		glGenVertexArrays(1, &surfaceVAO);
		glBindVertexArray(surfaceVAO);
		glGenBuffers(1, &surfaceVBO);
		glGenBuffers(1, &surfaceEBO);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfaceEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*eboVector.size(), &eboVector[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, surfaceVBO);
		glBufferData(GL_ARRAY_BUFFER, pointsSize+texCoordSize,
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, pointsSize, &allPoints[0]);
		glBufferSubData(GL_ARRAY_BUFFER, pointsSize, texCoordSize, &allTexCoords[0]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)pointsSize);
		glEnableVertexAttribArray(1);

		//glBindBuffer(GL_ARRAY_BUFFER, surfaceVBO);
		//glBufferData(GL_ARRAY_BUFFER, size, &allPoints[0], GL_STATIC_DRAW);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
	glPointSize(5);
	glBindVertexArray(surfaceVAO);
	glDrawArrays(GL_POINTS, 0, allPoints.size());
	glDrawElements(GL_TRIANGLES,EBOCount , GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPointSize(1);
}

int BezierSurface::VectorSizeByte(std::vector<glm::vec3> vector)
{
	return sizeof(float)*vector.size() * 3;
}