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
	PetViewer();
	~PetViewer();
	void ReadFile();

	void DrawAllTriangles();
	void SelectPointsForBezier();

	void DrawBezierSurface();

	unsigned int VAO = 0, VBO = 0, EBO = 0;;
	unsigned int surfaceVAO=0, surfaceVBO=0, surfaceEBO=0;

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
	std::vector<glm::vec3> allPointsArr[LENGTH];
	std::vector<unsigned int> eboVector;
	BezierSurface* surfaceVec[LENGTH];
};

