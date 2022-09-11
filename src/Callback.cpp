#include "Callback.h"
#include "Main.h"

// Resize callback for the window
void Callback::Resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowHeight = height;
	windowWidth = width;
	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	camera.Projection(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}


float xLastPos{};
float yLastPos{};
bool FirstLaunched{ true };

// Mouse movement callback for the window
void Callback::MouseMove(GLFWwindow* window, double xPos, double yPos)
{
	if (FirstLaunched)
	{
		xLastPos = static_cast<float>(xPos);
		yLastPos = static_cast<float>(yPos);
		FirstLaunched = false;
	}

	float xDiff{ static_cast<float>(xPos - xLastPos) };
	float yDiff{ static_cast<float>(yLastPos - yPos) };
	xLastPos = static_cast<float>(xPos);
	yLastPos = static_cast<float>(yPos);

	constexpr float sensivity{ 0.1f };
	xDiff *= sensivity;
	yDiff *= sensivity;

	camera.Rotate(xDiff, yDiff);
}

void Callback::MouseScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.Zoom(yOffset);
}