#include "Main.h"

#include <iostream>
#include <stdexcept>

GLFWwindow* mainWindow { nullptr };
extern int windowWidth { 800 };
extern int windowHeight{ 600 };

// Initializes the graphics routine
// Exceptions: [runtime_error]
static void GraphicsInit()
{
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("Error: glfwInit() unsuccessful");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(windowWidth, windowHeight, "ArlekinGame", nullptr, nullptr);
	if (mainWindow == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Error: failed co create a window");
	}

	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Error: failed to load GLAD");
	}

	glViewport(0, 0, windowWidth, windowHeight);
}

int main()
{
	try
	{
		GraphicsInit();
		Run();
	}

	catch (const std::runtime_error& except)
	{
		std::cout << "Runtime exception has occured:\n" << except.what() << std::endl;
	}

	catch (const std::exception& except)
	{
		std::cout << "General exception has occured:\n" << except.what() << std::endl;
	}
	
	return 0;
}

