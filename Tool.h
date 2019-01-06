#pragma once
#include<vector>	
#include "glm/glm.hpp"

class Manager{
public:
	static std::vector<glm::vec3*> controlPointsManager;
};


int VectorSizeByte(std::vector<glm::vec3> vector);

unsigned int loadTexture(char const * path);

float Length(glm::vec2 a, glm::vec2 b);
