#include "RenderLoop.h"
#include "Timer.h"
#include "Shader.h"
#include "Callback.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

static bool GraphicsInit();
static void ProcessInput(GLFWwindow* window);
static glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation);

static GLFWwindow* mainWindow{ nullptr };
int windowWidth{ 800 };
int windowHeight{ 600 };

std::string ShaderPath{ "shaders\\" };
std::string vertSource{ ShaderPath + "basic.vert" };
std::string fragSource{ ShaderPath + "basic.frag" };

glm::vec3 boxPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 boxRotation = glm::vec3(glm::radians(45.0f), 0.0f, 0.0f);
glm::vec3 boxScale = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraScale = glm::vec3(1.0f);
glm::vec3 cameraRotation = glm::vec3(0.0f);

glm::mat4 cameraView = glm::mat4(1.0f);
glm::mat4 cameraProjection = glm::mat4(1.0f);

// Runs the main loop of a game
int Run()
{
	if (!GraphicsInit())
	{
		std::cout << "Error: GLFW or GLAD initialization unsuccessful\n";
		return -1;
	}

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

	glEnable(GL_DEPTH_TEST);

	unsigned int VAO{};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO{};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*unsigned int EBO{};	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	Timer performanceTimer{};
	Timer transformTimer{};
	Shader shader{ vertSource, fragSource };
	Texture container{ "container.jpg" };
	Texture face{ "awesomeface.png", GL_RGBA };

	shader.Use();
	shader.SetInt("texSample1", 0);
	shader.SetInt("texSample2", 1);

	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat4 boxModel = glm::mat4(1.0f);

	boxModel = glm::translate(boxModel, boxPosition);
	boxModel = glm::scale(boxModel, glm::vec3(1.0f));
	boxModel = glm::rotate(boxModel, boxRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	boxModel = glm::rotate(boxModel, boxRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	boxModel = glm::rotate(boxModel, boxRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	//projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), 0.10f, 100.0f);
	//or
	cameraProjection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(mainWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		performanceTimer.Reset();
		ProcessInput(mainWindow);

		cameraView = Transform(cameraPos, cameraScale, cameraRotation);

		shader.Use();
		container.Bind(GL_TEXTURE0);
		face.Bind(GL_TEXTURE1);
		glBindVertexArray(VAO);

		for (int i{ 0 }; i < cubePositions.size(); ++i)
		{
			boxModel = Transform(cubePositions[i], boxScale, 14.5f * i * glm::vec3(1.0f, 0.3f, 0.5f));

			shader.SetMat4f("model", glm::value_ptr(boxModel));
			shader.SetMat4f("view", glm::value_ptr(cameraView));
			shader.SetMat4f("projection", glm::value_ptr(cameraProjection));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
		std::cout << "FPS: [" << 1.0 / performanceTimer.Elapsed() << "]\n";
	}

	glfwTerminate();
	return 0;
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

static void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowHeight = height;
	windowWidth = width;
	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	cameraProjection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

static bool GraphicsInit()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "Error: glfwInit() unsuccessful" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(windowWidth, windowHeight, "ArlekinGame", nullptr, nullptr);
	if (mainWindow == nullptr)
	{
		std::cout << "Error: failed co create a window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error: failed to load GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(mainWindow, Resize);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	return true;
}

