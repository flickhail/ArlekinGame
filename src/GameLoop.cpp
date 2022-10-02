#include "GameLoop.hpp"
#include "Timer.hpp"
#include "GameEngine/Shader.hpp"
#include "GameEngine/Texture.hpp"
#include "GameEngine/CameraOLD.hpp"
#include "GameEngine/Camera2D.hpp"
#include "GameEngine/Renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <vector>
#include <filesystem>

namespace GameEngine
{
	static void ProcessInput(GLFWwindow* window);
	static glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation);
	void RenderObjects(const Shader& shader);

	namespace WindowEvent
	{
		static void Resize(GLFWwindow* window, int width, int height);
		static void MouseMove(GLFWwindow* window, double xPos, double yPos);
		static void MouseScroll(GLFWwindow* window, double xOffset, double yOffset);
	}

	const std::string ShaderPath	{ "src/shaders/" };
	const std::string ResourcesPath	{ "resources/" };
	const std::string vertBasic		{ ShaderPath + "basic.vert" };
	const std::string fragBasic		{ ShaderPath + "basic.frag" };

	Camera2D camera2D;

	float deltaTime{};
	int windowWidth{};
	int windowHeight{};

	// Initializes the graphics routine
	// Exceptions: [runtime_error]
	static GLFWwindow* GraphicsInit(int winWidth, int winHeight)
	{
		if (glfwInit() == GLFW_FALSE)
			throw std::runtime_error("GraphicsInit error: glfwInit() unsuccessful");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* mainWindow = glfwCreateWindow(winWidth, winHeight, "ArlekinGame", nullptr, nullptr);
		if (mainWindow == nullptr)
		{
			glfwTerminate();
			throw std::runtime_error("GraphicsInit error: failed co create a window");
		}

		glfwMakeContextCurrent(mainWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			throw std::runtime_error("GraphicsInit error: failed to load GLAD");
		}

		glViewport(0, 0, winWidth, winHeight);
		return mainWindow;
	}


	// Runs the main loop of a game
	int Run(int winWidth, int winHeight)
	{
		windowHeight = winHeight;
		windowWidth = winWidth;

		GLFWwindow* mainWindow = GraphicsInit(windowWidth, windowHeight);
		glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetFramebufferSizeCallback(mainWindow, WindowEvent::Resize);
		glfwSetCursorPosCallback(mainWindow, WindowEvent::MouseMove);
		glfwSetScrollCallback(mainWindow, WindowEvent::MouseScroll);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		float cubeSingle[] =
		{
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeSingle), cubeSingle, GL_STATIC_DRAW);

		unsigned int cubeEBO{};
		glGenBuffers(1, &cubeEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		camera2D = Camera2D{ { 0.0f, 0.0f, 3.0f }, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f };

		Timer<float> performanceTimer{};
		Timer<float> globalTimer{};
		Texture container{ ResourcesPath + "container.jpg" };
		Texture face{ ResourcesPath + "awesomeface.png", GL_RGBA };

		Shader shader{ vertBasic,   fragBasic };
		shader.Use();
		shader.SetInt("texSample1", 0);
		shader.SetInt("texSample2", 1);

		float lastFrame{};
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(mainWindow))
		{
			performanceTimer.Reset();
			//std::cout << camera2D;

			float currentFrame = globalTimer.Elapsed();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			ProcessInput(mainWindow);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//glm::mat4 view = camera.LookAt(camera.Position() + camera.Front());
			//camera.Transform2D(glm::vec3{ 0.0f });

			glm::mat4 boxModel = Transform(glm::vec3{ 0.0f }, glm::vec3{ 1.0f }, glm::vec3{ 0.0f });
			//glm::mat4 camProj = glm::ortho(-(float)windowWidth/2.0f, (float)windowWidth/2.0f, -(float)windowHeight/2.0f, (float)windowHeight/2.0f, 0.1f, 100.0f);
			//camProj = glm::mat4{ 1.0f };

			shader.Use();
			shader.SetMat4f("model", glm::value_ptr(boxModel));
			shader.SetMat4f("view", glm::value_ptr(camera2D.View()));
			shader.SetMat4f("projection", glm::value_ptr(camera2D.Projection()));
			
			glBindVertexArray(cubeVAO);
			container.Bind(GL_TEXTURE0);
			face.Bind(GL_TEXTURE1);
			RenderObjects(shader);
			glBindVertexArray(0);

			glfwSwapBuffers(mainWindow);
			glfwPollEvents();
			//std::cout << "FPS: [" << 1.0 / performanceTimer.Elapsed() << "]\n";
		}

		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);

		glfwTerminate();
		return 0;
	}

	void RenderObjects(const Shader& shader)
	{
		shader.Use();

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

		camera2D.Speed(2.5f * deltaTime);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera2D.Move(-camera2D.Right() * camera2D.Speed());

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera2D.Move(camera2D.Right() * camera2D.Speed());

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera2D.Move(camera2D.Up() * camera2D.Speed());

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera2D.Move(-camera2D.Up() * camera2D.Speed());
	}


	// Resize callback for the window
	static void WindowEvent::Resize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		windowHeight = height;
		windowWidth = width;
		camera2D.AspectRatio((float)width / (float)height);
	}

	// Mouse movement callback for the window
	static void WindowEvent::MouseMove(GLFWwindow* window, double xPos, double yPos)
	{

		static float xLastPos{ static_cast<float>(xPos) };
		static float yLastPos{ static_cast<float>(yPos) };

		float xDiff{ static_cast<float>(xPos - xLastPos) };
		float yDiff{ static_cast<float>(yLastPos - yPos) };
		xLastPos = static_cast<float>(xPos);
		yLastPos = static_cast<float>(yPos);

		constexpr float sensivity{ 0.1f };
		xDiff *= sensivity;
		yDiff *= sensivity;

		//camera.Rotate(xDiff, yDiff);
	}

	static void WindowEvent::MouseScroll(GLFWwindow* window, double xOffset, double yOffset)
	{
		static float scale{ 1.0f };

		scale -= (float)yOffset * 0.1f;

		camera2D.Scale({ scale, scale });
	}
}
