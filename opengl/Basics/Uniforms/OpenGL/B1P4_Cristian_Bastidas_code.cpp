#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Biblioteca adicional
#include <learnopengl/shader_s.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

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

	// Se cargan los Shader desde los archivos
	Shader vf_shader("shaders/B1P4.vs", "shaders/B1P4.fs");

	// Vertices junto con sus colores
	float vertices[] = {
		-0.55f, 0.6f, 0.0f, 0.7f, 0.8f, 0.2f,
		-0.55f, 0.2f, 0.0f, 0.6f, 0.9f, 0.2f,
		-0.15f, 0.2f, 0.0f, 0.6f, 0.8f, 0.3f,
		-0.55f, 0.6f, 0.0f, 0.3f, 0.8f, 0.4f,
		0.25f, 0.6f, 0.0f, 0.2f, 0.9f, 0.4f,
		-0.15f, 0.2f, 0.0f, 0.2f, 0.8f, 0.5f,
		-0.55f, 0.2f, 0.0f, 0.0f, 0.8f, 0.0f,
		-0.15f, -0.2f, 0.0f, 1.0f, 0.9f, 0.0f,
		0.25f, 0.2f, 0.0f, 1.0f, 0.8f, 0.1f,
		0.25f, 0.2f, 0.0f, 0.6f, 0.4f, 0.8f,
		-0.05f, -0.1f, 0.0f, 0.4f, 0.5f, 0.8f,
		0.25f, -0.1f, 0.0f, 0.4f, 0.4f, 0.9f,
		0.25f, 0.2f, 0.0f, 0.5f, 0.4f, 0.8f,
		0.25f, -0.1f, 0.0f, 0.4f, 0.5f, 0.8f,
		0.55f, 0.2f, 0.0f, 0.4f, 0.4f, 0.9f,
		-0.25f, -0.1f, 0.0f, 0.3f, 0.6f, 1.0f,
		-0.45f, -0.3f, 0.0f, 0.2f, 0.7f, 1.0f,
		-0.25f, -0.5f, 0.0f, 0.2f, 0.6f, 0.0f,
		-0.25f, -0.1f, 0.0f, 0.0f, 0.6f, 0.2f,
		-0.05f, -0.3f, 0.0f, 1.0f, 0.7f, 0.2f,
		-0.25f, -0.5f, 0.0f, 1.0f, 0.6f, 0.3f,
		0.25f, -0.1f, 0.0f, 0.0f, 0.4f, 0.4f,
		0.05f, -0.3f, 0.0f, 1.0f, 0.5f, 0.4f,
		0.25f, -0.5f, 0.0f, 1.0f, 0.4f, 0.5f,
		0.25f, -0.1f, 0.0f, 0.0f, 0.4f, 0.4f,
		0.45f, -0.3f, 0.0f, 1.0f, 0.5f, 0.4f,
		0.25f, -0.5f, 0.0f, 1.0f, 0.4f, 0.5f,
	};

	// Numero de triangulos a dibujar

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(24.0f / 255.0f, 24.0f / 255.0f, 27.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Tiempo
		float time_val = glfwGetTime();

		// Se obtiene un valor para la posición en x de forma oscilatoria
		float x_value = (sin(time_val) / 3.0f);

		// Se cambian los colores
		float R = (cos(time_val) / 2.0f) + 0.5f;
		float G = (cos(sin(time_val)) / 2.0f) + 0.5f;
		float B = (sin(time_val) / 2.0f) + 0.5f;
		
		// Usar el shader program
		// vf_shader.setVec3("RGB", R, G, B);
		vf_shader.use();
		vf_shader.setFloat("movex", x_value);	// Mover en x

		vf_shader.setFloat("R", R);		// Cambiar los colores
		vf_shader.setFloat("G", G);		// Cambiar los colores
		vf_shader.setFloat("B", B);		// Cambiar los colores


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 27);

		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
