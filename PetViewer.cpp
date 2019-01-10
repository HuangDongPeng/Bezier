#include "pch.h"
#include "PetViewer.h"
#include <iostream>
#include <fstream>

using namespace std;


float Max(float a, float b) {
	return a > b ? a : b;
}

void PetViewer::ReadFile() {

	//读文件
	ifstream inFile("pet_raw1.dat", ios::in | ios::binary);
	if (!inFile) {
		cout << "error" << endl;
		return;
	}

#pragma region init
	char* tmp = (char*)malloc(4);
	float tmpPos[3];
	int xCount = 0;
	int yCount = 0;
	int zCount = 0;

	float step = 2.0 / RAWNUM;
	float xScreen = -1;
	float yScreen = 1;
	std::vector<glm::vec3> *tmpVec = new vector<glm::vec3>();
#pragma endregion

	glm::vec3 pointForBezier;
	std::vector<glm::vec3> vecForBezier;
	std::vector<BezierCurver*> bezierCurvers;

	while (inFile.read(tmp, sizeof(float)))
	{
		float *tmpFloat = (float*)tmp;
		(*tmpVec).push_back(glm::vec3(xScreen, yScreen, *tmpFloat));
		xScreen += step;
		xCount++;

		if (xCount % 20 == 0 && yCount % 20 == 0) {
			vecForBezier.push_back(glm::vec3(xScreen, yScreen, *tmpFloat));
		}

		if (xCount == RAWNUM)
		{
			if (vecForBezier.size() != 0) {
				BezierCurver* newBezierCurver= new BezierCurver(vecForBezier, 0.1f);
				bezierCurvers.push_back(&(*newBezierCurver));
				vecForBezier.clear();
			}

			//读完一行
			xScreen = -1;
			yScreen -= step;
			xCount = 0;
			yCount++;
			if (yCount == COWNUM)
			{
				//读完一面
				surfaceVec[zCount] = new BezierSurface(bezierCurvers);
				bezierCurvers.clear();
				yScreen = 1;
				allPointsArr[zCount] = *tmpVec;
				tmpVec = new vector<glm::vec3>();
				yCount = 0;
				zCount++;


				if (zCount == LENGTH) {
					break;
				}
			}
		}
	}
	//设置 EBO
	int raw = 0;
	for (int j = 0; j < RAWNUM - 1; j++) {
		int index = j * RAWNUM;
		for (int i = 0; i < COWNUM - 1; i++) {
			eboVector.push_back(index);
			eboVector.push_back(index + 1);
			eboVector.push_back(index + RAWNUM);

			eboVector.push_back(index + 1);
			eboVector.push_back(index + 1 + RAWNUM);
			eboVector.push_back(index + RAWNUM);
			index++;
		}
	}
	free(tmp);
	inFile.close();
}

//
void PetViewer::DrawAllTriangles() {
	
	if (VAO == 0) {
		glGenVertexArrays(1, &VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*eboVector.size(), &eboVector[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, allPointsArr[curZ].size() * sizeof(glm::vec3), &(allPointsArr[curZ])[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allPointsArr[curZ].size() * sizeof(glm::vec3), &(allPointsArr[curZ])[0], GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, eboVector.size(), GL_UNSIGNED_INT,0);
	//glDrawArrays(GL_POINTS, 0, allPoints.size());
	glBindVertexArray(0);
}


void PetViewer::DrawBezierSurface() {
	//bezierSurface->DrawSurfaceStatic();
	surfaceVec[curZ]->DrawSurfaceStatic();
}


PetViewer::PetViewer()
{
}


PetViewer::~PetViewer()
{
}
