#include "RenderLoop.h"
#include "Timer.h"
#include "Callback.h"

static GLFWwindow* window{ nullptr };
constexpr const char* vertShSource =
{
"#version 330 core\n"

"layout(location = 0) in vec3 aPos;\n"

"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n"
};

constexpr const char* fragShSource =
{
"#version 330 core\n"

"out vec4 FragColor;\n"

"void main()\n"
"{\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n"
};

void Run()
{
	if (!Init())
	{
		std::cout << "Error: initialization unsuccessful\n";
		return;//-1;
	}

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	unsigned int VAO{};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO{};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int vertShader{ glCreateShader(GL_VERTEX_SHADER) };
	glShaderSource(vertShader, 1, &vertShSource, nullptr);
	glCompileShader(vertShader);

	unsigned int fragShader{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(fragShader, 1, &fragShSource, nullptr);
	glCompileShader(fragShader);

	CheckShaderCompilation(vertShader);
	CheckShaderCompilation(fragShader);

	unsigned int shaderProgram{};
	shaderProgram = glCreateProgram();
	if (shaderProgram == 0)
	{
		std::cout << "Error: glCreateProgram() returned 0 value\n";
		return;//-1;
	}

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	int success_status{};
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_status);
	if (!success_status)
	{
		char infoLog[512]{};
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	glUseProgram(shaderProgram);
	Timer timer{};
	while (!glfwWindowShouldClose(window))
	{
		timer.Reset();
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);



		glfwSwapBuffers(window);
		glfwPollEvents();

		std::cout << "FPS: [" << 1.0 / timer.Elapsed() << "]\n";
	}

	glfwTerminate();
}

bool Init()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "Error: glfwInit() unsuccessful" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1000, 600, "ArlekinGame", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Error: failed co create window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error: failed to load GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, 1000, 600);
	glfwSetFramebufferSizeCallback(window, Callback::Resize);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	return true;
}

void CheckShaderCompilation(int shader)
{
	int success_status{};
	char infoLog[512]{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success_status);
	if (!success_status)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Error: shader compilation, see infoLog...\n" << infoLog << std::endl;
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}