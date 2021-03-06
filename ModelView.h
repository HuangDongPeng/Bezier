#pragma once
#include "Shader.h"
#include <vector>
#include "glm/glm.hpp"
#include "Camera.h"
#include "ConcentrationDataManager.h"

class ModelView:public ISyncViews
{
public:
	enum Views
	{
		none,
		front,
		back,
		left,
		right,
		up,
		down
	};
public:
	ModelView(int IN_row,int IN_col,int IN_length,float IN_step);
	~ModelView();
	void RenderMesh(Shader shader, int selectedPrimID=-1,int selectedViews=0);
	void RenderCubeInSelectedPos(Views views, int primID,Shader shader);

	void SyncViews(glm::vec3 concPos);

private:
	void SpawnVertex(int rowVertexNum, int colVertexNum, int lengthVertexNum);
	void InitVAO(unsigned int &vao, unsigned int &eboCount,int row, int col, Views views);
	void RenderCube(Shader shader);
	glm::vec3 CalcConcentrationsPos(Views views, int primID);

private:

	int row;
	int col;
	int length;

	int rowVertexNum;
	int colVertexNum;
	int lengthVertexNum;

	float step;

	unsigned int frontVAO=0,frontEBOCount;
	unsigned int upVAO=0, upEBOCount;
	unsigned int leftVAO =0, leftEBOCount;
	unsigned int cubeVAO = 0;

	Views curSelectedView;
	int curSelectedPrimID=0;
	glm::vec3 curConcPos;
};

