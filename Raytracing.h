#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Tool.h"

class Raytracing
{
private:
	glm::mat4* projection;
	glm::mat4* view;
	Camera* camera;

	unsigned int framebuffer = 0;
	unsigned int textureColorbuffer;
	unsigned int quadVAO = 0, quadVBO = 0;

public:
	Raytracing(int SCR_WIDTH, int SCR_HEIGHT, glm::mat4& projection_IN, glm::mat4& view_IN,Camera& camera_IN);
	~Raytracing();
	Shader* raytracingShader;
	Shader* squadShader;

	void Draw();




};

