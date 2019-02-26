#include "pch.h"
#include "PetViewer.h"
#include <iostream>
#include <fstream>
using namespace std;


void PetViewer::ReadFile(bool isBezier) {
	test(testArr);
	//读文件
	ifstream inFile("pet_raw1.dat",ios::in | ios::binary);
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
		
		if (isBezier&&xCount % 20 == 0 && yCount % 20 == 0) {
			vecForBezier.push_back(glm::vec3(xScreen, yScreen, *tmpFloat));
		}

		xScreen += step;
		xCount++;
		if (xCount == RAWNUM)
		{
			if (vecForBezier.size() != 0) {
				BezierCurver* newBezierCurver = new BezierCurver(vecForBezier, 0.05f);
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


	InitVector();
	InitBezierSurface(20, 20, 0.1f);
}

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
	glDrawArrays(GL_POINTS, 0, allPointsArr[curZ].size());
	glBindVertexArray(0);
}

void PetViewer::DrawBezierSurface() {
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

PetViewer::~PetViewer() {}

void PetViewer::test(unsigned int *test)
{
	cout << test[2] << endl;
}

#pragma region newCode
void PetViewer::InitVector()
{
	float xStep = 2.0 / LENGTH;
	float yStep = 2.0 / RAWNUM;
	float xScreen = -1;
	float yScreen = 1;
	int xCount = 0;
	int yCount = 0;
	//Init right view points vector
	for (int z_rightView = 0; z_rightView < RAWNUM; z_rightView++) {
		vector<glm::vec3>* tmpVec = new vector<glm::vec3>();
		for (int cow = 0; cow < COWNUM; cow++) {
			for (int index = LENGTH - 1; index >= 0; index--) {
				float x, y, z;
				x = xScreen;
				y = yScreen;
				z = allPointsArr[index][cow*RAWNUM+ z_rightView].z;
				tmpVec->push_back(glm::vec3(x,y,z));
				xScreen += xStep;
			}
			xScreen = -1;
			yScreen -= yStep;
		}
		xScreen = -1;
		yScreen = 1;
		allPointsArr_right[z_rightView] = *tmpVec;
	}
	//Init Vertical view vector
	for (int z_verticalView = 0; z_verticalView < COWNUM; z_verticalView++) {
		vector<glm::vec3>* tmpVec = new vector<glm::vec3>();
		for (int cow = 0; cow < RAWNUM; cow++) {
			for (int index = LENGTH - 1; index >= 0; index--)
			{
				float x, y, z;
				x = xScreen;
				y = yScreen;
				z = allPointsArr[index][RAWNUM-1-cow+(z_verticalView*RAWNUM)].z;
				tmpVec->push_back(glm::vec3(x, y, z));
				xScreen += xStep;
			}
			xScreen = -1;
			yScreen -= yStep;
		}
		xScreen = -1;
		yScreen = 1;
		allPointsArr_vertical[z_verticalView] = *tmpVec;
	}
}

void PetViewer::InitBezierSurface(int intervalCow,int intervalRaw,float lerp)
{
	//right view
	for (int z_rightView = 0; z_rightView < COWNUM; z_rightView++) {
		std::vector<BezierCurver*> curvers;
		for (int raw = 0; raw < RAWNUM; raw++) {
			if (raw % intervalRaw != 0)
				continue;

			std::vector<glm::vec3> pos;
			for (int index = 0; index < LENGTH;index++) {
				if (index % intervalCow == 0)
					pos.push_back(allPointsArr_right[z_rightView][index+raw*LENGTH]);
			}
			 BezierCurver* newCurver=new BezierCurver(pos, lerp);
			 curvers.push_back(newCurver);
		}
		surfaceVec_right[z_rightView] = new BezierSurface(curvers);
	}

	//vertical view
	for (int z_verticalView = 0; z_verticalView < RAWNUM; z_verticalView++) {
		std::vector<BezierCurver*> curvers;
		for (int raw = 0; raw < COWNUM; raw++) {
			if (raw % intervalRaw != 0)
				continue;

			std::vector<glm::vec3> pos;
			for (int index = 0; index < LENGTH; index++) {
				if (index % intervalCow == 0)
					pos.push_back(allPointsArr_vertical[z_verticalView][index+raw*LENGTH]);
			}
			BezierCurver* newCurver = new BezierCurver(pos, lerp);
			curvers.push_back(newCurver);
		}
		surfaceVec_vertical[z_verticalView] = new BezierSurface(curvers);
	}
}

void PetViewer::DrawAllPointsRightView()
{
	if (rightViewPointVAO == 0) {
		glGenVertexArrays(1, &rightViewPointVAO);

		glGenBuffers(1, &rightViewPointVBO);
		glBindVertexArray(rightViewPointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rightViewPointVBO);
		glBufferData(GL_ARRAY_BUFFER, allPointsArr_right[curZ_otherView].size() * sizeof(glm::vec3), &(allPointsArr_right[curZ_otherView])[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(rightViewPointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rightViewPointVBO);
	glBufferData(GL_ARRAY_BUFFER, allPointsArr_right[curZ_otherView].size() * sizeof(glm::vec3), &(allPointsArr_right[curZ_otherView])[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, allPointsArr_right[curZ_otherView].size());
	glBindVertexArray(0);
}

void PetViewer::DrawAllPointsVerticalView()
{
	if (verticalViewPointVAO == 0) {
		glGenVertexArrays(1, &verticalViewPointVAO);

		glGenBuffers(1, &verticalViewPointVBO);
		glBindVertexArray(verticalViewPointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, verticalViewPointVBO);
		glBufferData(GL_ARRAY_BUFFER, allPointsArr_vertical[curZ_otherView].size() * sizeof(glm::vec3), &(allPointsArr_vertical[curZ_otherView])[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(verticalViewPointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, verticalViewPointVBO);
	glBufferData(GL_ARRAY_BUFFER, allPointsArr_vertical[curZ_otherView].size() * sizeof(glm::vec3), &(allPointsArr_vertical[curZ_otherView])[0], GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, allPointsArr_vertical[curZ_otherView].size());
	glBindVertexArray(0);
}

void PetViewer::DrawBezierSurfaceRightView()
{
	if (surfaceVec_right[curZ_otherView] != nullptr) {
		surfaceVec_right[curZ_otherView]->DrawSurfaceStatic();
	}
}

void PetViewer::DrawBezierSurfaceVerticalView()
{
	if (surfaceVec_vertical[curZ_otherView] != nullptr) {
		surfaceVec_vertical[curZ_otherView]->DrawSurfaceStatic();
	}
}
#pragma endregion


