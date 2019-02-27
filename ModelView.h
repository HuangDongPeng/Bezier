#pragma once
#include "Shader.h"
#include <vector>
#include "glm/glm.hpp"

class ModelView
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
	glm::vec3 CalcRealPos(Views views,int primID);


private:
	void SpawnVertex(int rowVertexNum, int colVertexNum, int lengthVertexNum);
	void InitVAO(unsigned int &vao, unsigned int &eboCount,int row, int col, Views views);

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


	


};

