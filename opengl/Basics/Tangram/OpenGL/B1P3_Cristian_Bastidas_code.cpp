#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 triangle_color;\n"
"void main()\n"
"{\n"
"   FragColor = triangle_color;\n"
"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tangram -  Imagen 5 - Cristian Bastidas - G8", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}


	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			<< infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float v0[] = {
		-0.55f, 0.6f, 0.0f,
		-0.55f, 0.2f, 0.0f,
		-0.15f, 0.2f, 0.0f,
	};

	float v1[] = {
		-0.55f, 0.6f, 0.0f,
		0.25f, 0.6f, 0.0f,
		-0.15f, 0.2f, 0.0f,
	};

	float v2[] = {
		-0.55f, 0.2f, 0.0f,
		-0.15f, -0.2f, 0.0f,
		0.25f, 0.2f, 0.0f,
	};

	float v3[] = {
		0.25f, 0.2f, 0.0f,
		-0.05f, -0.1f, 0.0f,
		0.25f, -0.1f, 0.0f,
	};

	float v4[] = {
		0.25f, 0.2f, 0.0f,
		0.25f, -0.1f, 0.0f,
		0.55f, 0.2f, 0.0f,
	};

	float v5[] = {
		-0.25f, -0.1f, 0.0f,
		-0.45f, -0.3f, 0.0f,
		-0.25f, -0.5f, 0.0f,
	};

	float v6[] = {
		-0.25f, -0.1f, 0.0f,
		-0.05f, -0.3f, 0.0f,
		-0.25f, -0.5f, 0.0f,
	};

	float v7[] = {
		0.25f, -0.1f, 0.0f,
		0.05f, -0.3f, 0.0f,
		0.25f, -0.5f, 0.0f,
	};

	float v8[] = {
		0.25f, -0.1f, 0.0f,
		0.45f, -0.3f, 0.0f,
		0.25f, -0.5f, 0.0f,
	};

	float colors[] = {
		0.6f, 0.8f, 0.2f,
		0.2f, 0.8f, 0.4f,
		1.0f, 0.8f, 0.0f,
		0.4f, 0.4f, 0.8f,
		0.4f, 0.4f, 0.8f,
		0.2f, 0.6f, 1.0f,
		1.0f, 0.6f, 0.2f,
		1.0f, 0.4f, 0.4f,
		1.0f, 0.4f, 0.4f,
	};

	// Numero de triangulos a dibujar
	const int n = sizeof(colors) / (3 * sizeof(float));

	float* vertices[] = { v0, v1, v2, v3, v4, v5, v6, v7, v8 };

	unsigned int VBO[n], VAO[n];
	glGenVertexArrays(n, VAO);
	glGenBuffers(n, VBO);


	for (int i = 0; i < n; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v0), vertices[i], GL_STATIC_DRAW);
		std::cout << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << " " << vertices[i][8]  << "\n";

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(24.0f / 255.0f, 24.0f / 255.0f, 27.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int vertexColorLocation = glGetUniformLocation(shaderProgram, "triangle_color");
		glUseProgram(shaderProgram);

		int j = 0;
		for (int i = 0; i < n; i++) {

			glUniform4f(vertexColorLocation, colors[i + j], colors[i + j + 1], colors[i + j + 2], 1.0f);

			glBindVertexArray(VAO[i]);
			//std::cout << VAO[i] << "\n";
			glDrawArrays(GL_TRIANGLES, 0, 3);
			j += 2;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(n, VAO);
	glDeleteBuffers(n, VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}