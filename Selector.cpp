#include "pch.h"
#include "Selector.h"
#include <iostream>
#include "Tool.h"

Selector::Selector(int screenWidth_IN,int screenHeight_IN,glm::mat4& view_IN,glm::mat4& projection_IN)
	:screenWidth(screenWidth_IN)
	,screenHeight(screenHeight_IN)
	,view(&view_IN)
	,projection(&projection_IN)
{
}


Selector::~Selector()
{
}

void Selector::Select(float screenX, float screenY, std::vector<glm::vec3*>& controlPointsManager, float xoffset, float yoffset) {
	if (selected == nullptr) {
		float NDSx = screenX * 2 / screenWidth - 1;
		float NDSy = 1 - screenY * 2 / screenHeight;
		glm::vec2 mouseNDS(NDSx, NDSy);
		for (std::vector<glm::vec3*>::iterator it = controlPointsManager.begin(); it < controlPointsManager.end(); it++) {
			glm::vec4 controlPoint = (*projection) * (*view) * glm::vec4(**it, 1.0f);
			float x = controlPoint.x / controlPoint.w;
			float y = controlPoint.y / controlPoint.w;
			glm::vec4 controlPointNDS = glm::vec4(x, y, controlPoint.z, controlPoint.w);

			float length = Length(controlPointNDS, mouseNDS);
			if (length < 0.1f) {
				selected = (*it);
				break;
			}
		}
	}
	else
	{
		glm::vec4 controlPoint = (*projection) * (*view) * glm::vec4(*selected, 1.0f);
		float x = controlPoint.x / controlPoint.w;
		float y = controlPoint.y / controlPoint.w;
		glm::vec4 controlPointNDS = glm::vec4(x, y, controlPoint.z, controlPoint.w);
		//像素坐标的位移转化为NDS坐标的位移
		float xoffsetNDS = xoffset * 2 / screenWidth;
		float yoffsetNDS = yoffset * 2 / screenHeight;
		controlPointNDS += glm::vec4(xoffsetNDS, yoffsetNDS, 0, 0);
		//转换为世界坐标的位移
		x = controlPointNDS.x*controlPointNDS.w;
		y = controlPointNDS.y*controlPointNDS.w;
		glm::vec4 newControlPoint = glm::vec4(x, y, controlPointNDS.z, controlPointNDS.w);
		newControlPoint = glm::inverse(*view)*glm::inverse(*projection)*newControlPoint;
		*selected = newControlPoint;
	}
	

}
