#pragma once
#include "Shader.h"
#include <vector>
#include "glm/glm.hpp"

class ModelView
{
public:
	ModelView(int IN_row,int IN_col,int IN_length,float IN_step);
	~ModelView();
	void RenderMesh(Shader shader, int selectedPrimID=-1);

private:
	void SpawnVertex(int rowVertexNum, int colVertexNum, int lengthVertexNum);

private:
	int row;
	int col;
	int length;

	int rowVertexNum;
	int colVertexNum;
	int lengthVertexNum;

	float step;

	unsigned int vao;
	std::vector<glm::vec3> vertexVec;
	std::vector<int> eboVec;


};

