#pragma once
#include "Tool.h"
#include <vector>
#include "BezierCurver.h"	
#include "BezierSurface.h"

#define RAWNUM 160
#define COWNUM 160
#define LENGTH 200
class PetViewer
{
public:
	PetViewer(glm::mat4 &projection, glm::mat4 &view,int screenWidth,int screenHeight);
	~PetViewer();
	void ReadFile(bool isBezier);

	void DrawAllTriangles();//正视图


	void CalculateScePos();
	void ShowCurMsg(float xpos, float ypos);
	void DrawBezierSurface();//正试图的贝塞尔曲面

	unsigned int VAO = 0, VBO = 0, EBO = 0;//正视图点的VAO
	int screenWidth;
	int screenHeight;


	void SetZ(int z) {
		if (z > LENGTH - 1) {
			curZ = LENGTH - 1;
			return;
		}
		if (z < 0) {
			curZ = 0;
			return;
		}
		curZ = z;
	}
	int GetZ() {
		return curZ;
	}
private:
	int curZ = 0;
	std::vector<glm::vec3> allPointsArr[LENGTH];//存储每一面
	std::vector<unsigned int> eboVector;

	BezierSurface* surfaceVec[LENGTH];//正视图

#pragma region NewCode
public:
	int curZ_otherView=0;
	void SetZ_otherView(int z) {
		if (z >= COWNUM) {
			curZ_otherView = COWNUM - 1;
			return;
		}
		if (z < 0) {
			curZ_otherView = 0;
			return;
		}
		curZ_otherView = z;
	}
	int GetZ_otherView() {
		return curZ_otherView;
	}
	std::vector<glm::vec3> allPointsArr_right[COWNUM];
	std::vector<glm::vec3> allPointsArr_vertical[RAWNUM];
	BezierSurface* surfaceVec_right[COWNUM];//正视图贝塞尔曲面
	BezierSurface* surfaceVec_vertical[RAWNUM];//正视图贝塞尔曲面

	void InitVector();
	void InitBezierSurface(int intervalCow, int intervalRaw, float lerp);
	void DrawAllPointsRightView();
	void DrawAllPointsVerticalView();
	void DrawBezierSurfaceRightView();//正试图的贝塞尔曲面
	void DrawBezierSurfaceVerticalView();//正试图的贝塞尔曲面

	unsigned int rightViewPointVAO = 0, rightViewPointVBO = 0;
	unsigned int verticalViewPointVAO = 0, verticalViewPointVBO = 0;
	
#pragma endregion

	unsigned int testArr[LENGTH] = { 1 };
	void test(unsigned int*);


	glm::mat4* projection;
	glm::mat4* view;

	glm::vec2 minPixelPos;
	glm::vec2 maxPixelPos;

};

