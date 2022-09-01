#include "Main.h"

#include <iostream>

GLFWwindow* mainWindow { nullptr };
extern int windowWidth { 800 };
extern int windowHeight{ 600 };

// Initializes the graphics routine
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
	return true;
}

int main()
{
	if (!GraphicsInit())
	{
		std::cout << "Error: Graphics routine couldn't initialize\n";
		return -1;
	}

	if (Run())
	{
		std::cout << "Error: Run() returned error\n";
		return -1;
	}
	
	return 0;
}

