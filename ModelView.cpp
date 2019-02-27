#include "pch.h"
#include "ModelView.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

ModelView::ModelView(int IN_row, int IN_col, int IN_length, float IN_step)
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
	std::vector<glm::vec3> vertexVec;
	std::vector<int> eboVec;

	InitVAO(frontVAO, frontEBOCount, rowVertexNum, colVertexNum, ModelView::Views::front);
	InitVAO(upVAO, upEBOCount, lengthVertexNum, colVertexNum, ModelView::Views::up);
	InitVAO(leftVAO, leftEBOCount, lengthVertexNum, rowVertexNum, ModelView::Views::left);


	//#pragma region UpFace
	//	eboVec.clear();
	//	vertexVec.clear();
	//	curVertex = glm::vec3(0);
	//	for (int length = 0; length < lengthVertexNum; length++)
	//	{
	//		for (int col = 0; col < colVertexNum; col++)
	//		{
	//			vertexVec.push_back(curVertex);
	//			curVertex.x += step;
	//		}
	//		curVertex.x = 0;
	//		curVertex.z -= step;
	//	}
	//
	//	//ÉèÖÃEBO
	//	int index = 0;
	//	for (int length = 0; length < lengthVertexNum-1; length++)
	//	{
	//		index = length * lengthVertexNum;
	//		for (int col = 0; col < colVertexNum - 1; col++) {
	//			eboVec.push_back(index);
	//			eboVec.push_back(index + 1);
	//			eboVec.push_back(index + colVertexNum);
	//
	//			eboVec.push_back(index + 1);
	//			eboVec.push_back(index + colVertexNum + 1);
	//			eboVec.push_back(index + colVertexNum);
	//
	//			index++;
	//		}
	//	}
	//	if (upVAO == 0) {
	//		unsigned int vbo, ebo;
	//		glGenVertexArrays(1, &upVAO);
	//		glBindVertexArray(upVAO);
	//		glGenBuffers(1, &vbo);
	//		glGenBuffers(1, &ebo);
	//
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*eboVec.size(), &eboVec[0], GL_STATIC_DRAW);
	//
	//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertexVec.size(), &vertexVec[0], GL_STATIC_DRAW);
	//
	//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//		glEnableVertexAttribArray(0);
	//
	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//		glBindVertexArray(0);
	//		upEBOCount = eboVec.size();
	//	}
	//#pragma endregion

#pragma region leftFace

#pragma endregion

}
void ModelView::InitVAO(unsigned int &vao, unsigned int &eboCount, int rowVertexNum, int colVertexNum, ModelView::Views views)
{
	glm::vec3 curVertex = glm::vec3(0);
	std::vector<glm::vec3> vertexVec;
	std::vector<int> eboVec;
	for (int row = 0; row < rowVertexNum; row++)
	{
		for (int col = 0; col < colVertexNum; col++)
		{
			vertexVec.push_back(curVertex);
			switch (views)
			{
			case ModelView::front:
				curVertex.x += step;
				break;
			case ModelView::left:
				curVertex.y -= step;
				break;
			case ModelView::up:
				curVertex.x += step;
				break;
			default:
				break;
			}
		}
		switch (views)
		{
		case ModelView::front:
			curVertex.x = 0;
			curVertex.y -= step;
			break;
		case ModelView::left:
			curVertex.y = 0;
			curVertex.z -= step;
			break;
		case ModelView::up:
			curVertex.x = 0;
			curVertex.z -= step;
			break;
		default:
			break;
		}

	}

	//ÉèÖÃEBO
	int index = 0;
	for (int row = 0; row < rowVertexNum - 1; row++)
	{
		index = row * colVertexNum;
		for (int col = 0; col < colVertexNum - 1; col++) {
			eboVec.push_back(index);
			eboVec.push_back(index + 1);
			eboVec.push_back(index + colVertexNum);

			eboVec.push_back(index + 1);
			eboVec.push_back(index + colVertexNum + 1);
			eboVec.push_back(index + colVertexNum);

			index++;
		}
	}
	if (vao == 0) {
		unsigned int vbo, ebo;
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
		eboCount = eboVec.size();
	}
}

void ModelView::RenderMesh(Shader shader, int selectedPrimID, int selectedViews)
{

	shader.use();
	/*if (selectedPrimID != -1)
		shader.setInt("selectedPrimID", selectedPrimID);*/

	if (selectedPrimID != -1) {
		CalcRealPos(Views(selectedViews), selectedPrimID);
	}
	glm::mat4 model(1);

#pragma region front back
	shader.setInt("views", Views::front);
	glBindVertexArray(frontVAO);
	glDrawElements(GL_TRIANGLES, frontEBOCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader.setInt("views", Views::back);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -(step*length)));
	shader.setMat4("model", model);
	glBindVertexArray(frontVAO);
	glDrawElements(GL_TRIANGLES, frontEBOCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#pragma endregion

//#pragma region up down
//	model = glm::mat4(1);
//	shader.setMat4("model", model);
//	shader.setInt("views", Views::up);
//	glBindVertexArray(upVAO);
//	glDrawElements(GL_TRIANGLES, upEBOCount, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	model = glm::mat4(1);
//	shader.setInt("views", Views::down);
//	model = glm::translate(model, glm::vec3(0.0f, -(step*row), 0.0f));
//	shader.setMat4("model", model);
//	glBindVertexArray(upVAO);
//	glDrawElements(GL_TRIANGLES, upEBOCount, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//#pragma endregion
//
//#pragma region left right
//	model = glm::mat4(1);
//	shader.setMat4("model", model);
//	shader.setInt("views", Views::left);
//	glBindVertexArray(leftVAO);
//	glDrawElements(GL_TRIANGLES, leftEBOCount, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	model = glm::translate(model, glm::vec3(step*col, 0.0f, 0.0f));
//	shader.setMat4("model", model);
//	shader.setInt("views", Views::right);
//	glBindVertexArray(leftVAO);
//	glDrawElements(GL_TRIANGLES, leftEBOCount, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//#pragma endregion


}

glm::vec3 ModelView::CalcRealPos(Views views, int primID)
{
	glm::vec3 result(0);
	int tmpPrimID = primID - 1;
	tmpPrimID /= 2;
	switch (views)
	{
	case ModelView::front:
		result.z = 0;
		result.x = tmpPrimID % col;
		int tmp=tmpPrimID - result.x;
		result.y = tmp % col;
		break;
	case ModelView::back:
		result.z = length;
		break;
	case ModelView::left:
		result.x = 0;
		break;
	case ModelView::right:
		result.x = col;
		break;
	case ModelView::up:
		result.y = 0;
		break;
	case ModelView::down:
		result.y = row;
		break;
	default:
		break;
	}
	if (result.x < 0)
		result.x = 0;
	if (result.y < 0)
		result.y = 0;
	if (result.z < 0)
		result.z = 0;
	std::cout << "x: " << result.x;
	std::cout << "	y: " << result.y;
	std::cout << "	z: " << result.z << std::endl;
	return result;
}