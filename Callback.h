#pragma once
#include <glad/glad.h>
#include <glfw3.h>

namespace Callback
{
	void Resize(GLFWwindow* window, int width, int height);
	void MouseMove(GLFWwindow* window, double xPos, double yPos);
	void MouseScroll(GLFWwindow* window, double xOffset, double yOffset);
}