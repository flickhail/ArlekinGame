#include "Callback.h"

void Callback::Resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}