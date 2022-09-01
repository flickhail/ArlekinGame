#include "RenderLoop.h"
#include "Main.h"
#include "Timer.h"
#include "Shader.h"
#include "Callback.h"
#include "Texture.h"
#include "Camera.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <filesystem>

static void ProcessInput(GLFWwindow* window);
static glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation);
void RenderObjects(Shader& shader);
void RenderBiggerObjects(Shader& shader);

std::string ShaderPath	{ "shaders\\" };
std::string vertBasic	{ ShaderPath + "basic.vert" };
std::string fragBasic	{ ShaderPath + "basic.frag" };
std::string vertStencil	{ ShaderPath + "simpColor.vert" };
std::string fragStencil	{ ShaderPath + "simpColor.frag" };

glm::vec3 boxPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 boxRotation = glm::vec3(glm::radians(45.0f), 0.0f, 0.0f);
glm::vec3 boxScale = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f);

extern Camera camera{ cameraPos, cameraTarget, glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f };



std::vector<glm::vec3> cubePositions {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
};

float deltaTime{};

// Runs the main loop of a game
int Run()
{
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(mainWindow, Callback::Resize);
	glfwSetCursorPosCallback(mainWindow, Callback::MouseMove);
	glfwSetScrollCallback(mainWindow, Callback::MouseScroll);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	//};

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	
	
	unsigned int cubeVAO{};
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	unsigned int cubeVBO{};
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	Timer performanceTimer{};
	Timer globalTimer{};
	Shader shader		{ vertBasic,   fragBasic   };
	Shader shaderStencil{ vertStencil, fragStencil };

	Texture container	{ "container.jpg" };
	Texture face		{ "awesomeface.png", GL_RGBA };

	shader.Use();
	shader.SetInt("texSample1", 0);
	shader.SetInt("texSample2", 1);

	float lastFrame{};

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(mainWindow))
	{
		performanceTimer.Reset();

		float currentFrame = globalTimer.Elapsed();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(mainWindow);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glm::mat4 view = camera.LookAt(camera.Position() + camera.Front());
		glm::mat4 projection = camera.Projection();

		shader.Use();
		shader.SetMat4f("view", glm::value_ptr(view));
		shader.SetMat4f("projection", glm::value_ptr(projection));

		shaderStencil.Use();
		shaderStencil.SetMat4f("view", glm::value_ptr(view));
		shaderStencil.SetMat4f("projection", glm::value_ptr(projection));

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glBindVertexArray(cubeVAO);
		container.Bind(GL_TEXTURE0);
		face.Bind(GL_TEXTURE1);
		RenderObjects(shader);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(cubeVAO);
		container.Bind(GL_TEXTURE0);
		face.Bind(GL_TEXTURE1);
		RenderBiggerObjects(shaderStencil);

		glBindVertexArray(0);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
		//std::cout << "FPS: [" << 1.0 / performanceTimer.Elapsed() << "]\n";
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
	return 0;
}

void RenderBiggerObjects(Shader& shader)
{
	shader.Use();
	for (int i{ 0 }; i < cubePositions.size(); ++i)
	{
		glm::mat4 boxModel;
		boxModel = Transform(cubePositions[i], boxScale * 1.1f, 14.5f * i * glm::vec3(1.0f, 0.3f, 0.5f));

		shader.SetMat4f("model", glm::value_ptr(boxModel));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void RenderObjects(Shader& shader)
{
	shader.Use();
	for (int i{ 0 }; i < cubePositions.size(); ++i)
	{
		glm::mat4 boxModel;
		boxModel = Transform(cubePositions[i], boxScale, 14.5f * i * glm::vec3(1.0f, 0.3f, 0.5f));

		shader.SetMat4f("model", glm::value_ptr(boxModel));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

// translation - vector by which you want to change position
// scale - vector that represents scaling in each coordinates
// rotation - angles in radians (!) that represents rotation around corresponding axis
static glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation)
{
	glm::mat4 transform = glm::mat4(1.0f);

	transform = glm::translate(transform, translation);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	return transform;
}

// Processes the input on a given window
static void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Position(camera.Position() - cameraSpeed * camera.Right());

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Position(camera.Position() + cameraSpeed * camera.Right());

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Position(camera.Position() + cameraSpeed * camera.Front());

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Position(camera.Position() - cameraSpeed * camera.Front());
}




