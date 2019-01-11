#include "pch.h"
#include "PetViewer.h"
#include <iostream>
#include <fstream>

using namespace std;

bool isBezier = false;
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

		if (isBezier&&xCount % 20 == 0 && yCount % 20 == 0) {
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
				if (bezierCurvers.size() != 0) {
					surfaceVec[zCount] = new BezierSurface(bezierCurvers);
					bezierCurvers.clear();
				}

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
	if (surfaceVec[curZ] != nullptr) {
		surfaceVec[curZ]->DrawSurfaceStatic();
	}
}


PetViewer::PetViewer(glm::mat4 &projection_IN,glm::mat4 &view_IN,int screenWidth_IN,int screenHeight_IN)
	:projection(&projection_IN)
	,view(&view_IN)
	,screenWidth(screenWidth_IN)
	,screenHeight(screenHeight_IN)
{

}
//
void PetViewer::CalculateScePos() {
	glm::vec3 minPos = allPointsArr[0][0];
	glm::vec3 maxPos = allPointsArr[0][(RAWNUM*COWNUM)-1];
	
	minPos.z = 0;
	maxPos.z = 0;

	glm::vec4 minPosScr = (*projection) * (*view) * glm::vec4(minPos,1.0);
	glm::vec4 maxPosScr = (*projection) * (*view) * glm::vec4(maxPos, 1.0);

	float x = minPosScr.x / minPosScr.w;
	float y = minPosScr.y / minPosScr.w;
	glm::vec2 minPosNDC = glm::vec2(x, y);

	x = maxPosScr.x / maxPosScr.w;
	y = maxPosScr.y / maxPosScr.w;
	glm::vec2 maxPosNDC = glm::vec2(x, y);

	x = ((minPosNDC.x + 1)*screenWidth)/2;
	y = -((minPosNDC.y - 1)*screenHeight)/2;
	minPixelPos = glm::vec2(x, y);

	x = ((maxPosNDC.x + 1)*screenWidth) / 2;
	y = -((maxPosNDC.y - 1)*screenHeight) / 2;
	maxPixelPos = glm::vec2(x, y);

	std::cout << "min x pixed: " << minPixelPos.x;
	std::cout << "max x pixed: " << maxPixelPos.x;
	std::cout << "min y pixed: " << minPixelPos.y;
	std::cout << "max y pixed: " << maxPixelPos.y;
}

void PetViewer::ShowCurMsg(float xpos,float ypos) {
	if (xpos<minPixelPos.x || xpos>maxPixelPos.x)
		return;
	if (ypos<minPixelPos.y || ypos>maxPixelPos.y)
		return;
	float xStep = (maxPixelPos.x - minPixelPos.x) / RAWNUM;
	float yStep = (maxPixelPos.y - minPixelPos.y) / COWNUM;
	int xIndex = (xpos - minPixelPos.x) / xStep;
	int yIndex = (ypos - minPixelPos.y) / yStep;
	int index = (RAWNUM * yIndex) + xIndex;
	cout << " x: " <<xIndex;	
	cout << " y: " << yIndex;
	cout << " 浓度: " << allPointsArr[curZ][index].z;
	cout << " 当前层: " << curZ << endl;
}

PetViewer::~PetViewer()
{
}
