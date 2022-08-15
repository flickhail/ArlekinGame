#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>


bool Init();
void processInput(GLFWwindow* window);
void CheckShaderCompilation(int shader);