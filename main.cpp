
#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "BezierSurface.h"
#include <iostream>
#include <random>
#include "BezierCurver.h"
#include "Tool.h"
#include "Selector.h"
#include "Koch.h"
#include "Raytracing.h"

glm::mat4 projection;
glm::mat4 view;
//std::vector<glm::vec3*> controlPointManager;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
Selector selector(SCR_WIDTH, SCR_HEIGHT, view, projection);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
#pragma region WindowInit
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_DEPTH_TEST);

#pragma endregion

#pragma region InitShape
	std::vector<glm::vec3> pointsVector;
	unsigned int VBO, VAO;

	glm::vec3 controlPoint1(-1.0f, 1.0f, 1.0f);
	glm::vec3 controlPoint2(0.0f, 1.0f, 1.5f);
	glm::vec3 controlPoint3(1.0f, 1.0f, -1.0f);

	glm::vec3 controlPoint4(-1.0f, 0.0f, 0.0f);
	glm::vec3 controlPoint5(0.0f, 0.0f, 0.0f);
	glm::vec3 controlPoint6(1.0f, 0.0f, 0.0f);

	glm::vec3 controlPoint7(-1.0, 0.0f, 0.0f);
	glm::vec3 controlPoint8(0.0, 0.0f, 0.0f);
	glm::vec3 controlPoint9(1.0, 0.0f, 0.0f);

	Shader shader("Bezier.vs", "Bezier.fs");
	pointsVector.push_back(controlPoint1);
	pointsVector.push_back(controlPoint2);
	pointsVector.push_back(controlPoint3);
	//pointsVector.push_back(controlPoint4);

	BezierCurver bezierCurver1(pointsVector, 0.1f);

	pointsVector.clear();
	pointsVector.push_back(controlPoint4);
	pointsVector.push_back(controlPoint5);
	pointsVector.push_back(controlPoint6);
	//pointsVector.push_back(controlPoint7);


	BezierCurver bezierCurver2(pointsVector, 0.1f);

	pointsVector.clear();
	pointsVector.push_back(controlPoint7);
	pointsVector.push_back(controlPoint8);
	pointsVector.push_back(controlPoint9);
	BezierCurver bezierCurver3(pointsVector, 0.1f);

	pointsVector.clear();
	glm::vec3 kochPoint1(-1.0, 0.0f, 0.0f);
	glm::vec3 kochPoint2(1.0, 0.0f, 0.0f);
	glm::vec3 kochPoint3(0.0, 1.0f, 0.0f);
	pointsVector.push_back(kochPoint1);
	pointsVector.push_back(kochPoint2);
	pointsVector.push_back(kochPoint3);

	Koch koch(pointsVector, 1);
	unsigned char* pix = new unsigned char[SCR_WIDTH*SCR_HEIGHT * 3];
#pragma endregion

#pragma region BezierSurface
	std::vector<BezierCurver*> baseCurvers;
	baseCurvers.push_back(&bezierCurver1);
	baseCurvers.push_back(&bezierCurver2);
	baseCurvers.push_back(&bezierCurver3);

	BezierSurface bezierSurface(baseCurvers);

#pragma endregion
	Raytracing raytracing(SCR_WIDTH, SCR_HEIGHT, projection, view,camera);
	Tool tool;

	Shader blinPhoneShader("blinPhone.vs", "blinPhone.fs");
	Shader phoneShader("phone.vs", "phone.fs");
	Shader lambertShader("lambert.vs", "lambert.fs");
	Shader lightShader("light.vs", "light.fs");

	unsigned int smileTexture = loadTexture("awesomeface.png");
	unsigned int containerTexture = loadTexture("container.jpg");
	unsigned int container2Texture = loadTexture("container2.png");



	blinPhoneShader.use();
	blinPhoneShader.setInt("texture1", 2);

	phoneShader.use();
	phoneShader.setInt("texture1", 1);

	lambertShader.use();
	lambertShader.setInt("texture1", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, smileTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, container2Texture);

	glm::vec3 lightPos(5, 5, 5);
	glm::vec3 lightColor(0.5, 0.5, 0.5);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		glm::mat4 model(1);

		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		bezierSurface.DrawCurversControlPoints();
		bezierSurface.DrawCurvers();
		bezierSurface.DrawSurface();
		//bezierSurface.DrawSurface();

//		
//#pragma region lighting
//
//
//		blinPhoneShader.use();
//		blinPhoneShader.setMat4("model", model);
//		blinPhoneShader.setMat4("view", view);
//		blinPhoneShader.setMat4("projection", projection);
//		blinPhoneShader.setVec3("lightPos", lightPos);
//		blinPhoneShader.setVec3("lightColor", lightColor);
//		blinPhoneShader.setVec3("viewPos", camera.Position);
//
//		phoneShader.use();
//		model = glm::mat4(1);
//		model = glm::translate(model, glm::vec3(3, 0, 0));
//		phoneShader.setMat4("model", model);
//		phoneShader.setMat4("view", view);
//		phoneShader.setMat4("projection", projection);
//		phoneShader.setVec3("lightPos", lightPos);
//		phoneShader.setVec3("lightColor", lightColor);
//		phoneShader.setVec3("viewPos", camera.Position);
//
//		lambertShader.use();
//		model = glm::mat4(1);
//		model = glm::translate(model, glm::vec3(3, 3, 0));
//		lambertShader.setMat4("model", model);
//		lambertShader.setMat4("view", view);
//		lambertShader.setMat4("projection", projection);
//		lambertShader.setVec3("lightPos", lightPos);
//		lambertShader.setVec3("lightColor", lightColor);
//		lambertShader.setVec3("viewPos", camera.Position);
//
//		lightShader.use();
//		model = glm::mat4(1);
//		model = glm::translate(model, lightPos);
//		lightShader.setMat4("model", model);
//		lightShader.setMat4("view", view);
//		lightShader.setMat4("projection", projection);
//		lightShader.setVec3("lightColor", lightColor);
//
//		tool.RenderCube(blinPhoneShader);
//		tool.RenderCube(phoneShader);
//		tool.RenderCube(lambertShader);
//		tool.RenderCube(lightShader);
//		//raytracing.Draw();
//#pragma endregion
//
		processInput(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	return 0;

}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		selector.Select(xpos, ypos, Manager::controlPointsManager, xoffset, yoffset);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
		selector.ResetSelected();
	}


	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


