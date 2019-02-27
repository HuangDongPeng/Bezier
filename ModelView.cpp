#include "pch.h"
#include "ModelView.h"
#include "glad/glad.h"

ModelView::ModelView(int IN_row, int IN_col, int IN_length,float IN_step)
	:row(IN_row),
	col(IN_col),
	length(IN_length),
	step(IN_step)
{
	rowVertexNum = row + 1;
	colVertexNum = col + 1;
	lengthVertexNum = length + 1;

	SpawnVertex(rowVertexNum, colVertexNum, lengthVertexNum);
}


ModelView::~ModelView()
{
}

void ModelView::SpawnVertex(int rowVertexNum, int colVertexNum, int lengthVertexNum)
{

	glm::vec3 curVertex = glm::vec3(0);

	//生成第一个面
	for (int row = 0; row < rowVertexNum; row++) 
	{
		for (int col = 0; col < colVertexNum; col++)
		{
			vertexVec.push_back(curVertex);
			curVertex.x += step;
		}
		curVertex.x = 0;
		curVertex.y += step;
	}

	//设置EBO
	int index = 0;
	for (int row = 0; row < rowVertexNum-1; row++)
	{
		index = row * colVertexNum;
		for (int col = 0; col < colVertexNum-1; col++) {
			eboVec.push_back(index);
			eboVec.push_back(index+1);
			eboVec.push_back(index + colVertexNum);

			eboVec.push_back(index+1);
			eboVec.push_back(index + colVertexNum+1);
			eboVec.push_back(index + colVertexNum);

			index++;
		}
	}
}


void ModelView::RenderMesh(Shader shader, int selectedPrimID)
{
	if (vao == 0) {
		unsigned int vbo,ebo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*eboVec.size(), &eboVec[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertexVec.size(), &vertexVec[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	shader.use();
	if (selectedPrimID != -1)
		shader.setInt("selectedPrimID", selectedPrimID);
		//glUniform1ui(glGetUniformLocation(shader.ID, "selectedPrimID"), selectedPrimID);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, eboVec.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

