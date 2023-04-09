#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "learnopengl/stb_image.h"

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cristian Bastidas - 1726171042", NULL, NULL);
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

	Shader vf_shader("shaders/vertexshader.vs", "shaders/fragmentshader.fs");

	float verticesA[] = {
		// Position			// Color			// Texture
		-0.55f, 0.6f, 0.0f,	1.0f, 0.8f, 0.0f,	0.0f, 0.4f,
		-0.55f, 0.2f, 0.0f,	0.2f, 0.8f, 0.4f,	0.0f, 0.0f,
		-0.15f, 0.2f, 0.0f,	0.2f, 0.6f, 1.0f,	0.4f, 0.0f,
		0.25f, 0.6f, 0.0f,  0.4f, 0.4f, 0.8f,	0.8f, 0.4f,
	};

	unsigned int indicesA[] = {
		0, 1, 2,
		0, 3, 2,
	};

	float verticesB[] = {
		// Position				// Color			// Texture
		-0.55f, 0.2f, 0.0f,		1.0f, 0.4f, 0.4f,	0.0f, 0.7f,
		-0.15f, -0.2f, 0.0f,	0.2f, 0.6f, 1.0f,	0.4f, 0.3f,
		0.25f, 0.2f, 0.0f,		0.2f, 0.8f, 0.4f,	0.8f, 0.7f,
		-0.05f, -0.1f, 0.0f,	0.7f, 0.8f, 0.2f,	0.5f, 0.4f,
		0.25f, -0.1f, 0.0f,		0.7f, 0.8f, 0.2f,	0.8f, 0.4f,
		0.55f, 0.2f, 0.0f,		1.0f, 0.8f, 0.0f,	1.0f, 0.7f,
		-0.25f, -0.1f, 0.0f,	0.7f, 0.8f, 0.2f,	0.3f, 0.4f,
		-0.45f, -0.3f, 0.0f,	0.0f, 0.8f, 0.2f,	0.1f, 0.2f,
		-0.25f, -0.5f, 0.0f,	0.7f, 1.0f, 0.2f,	0.3f, 0.0f,
		-0.05f, -0.3f, 0.0f,	0.7f, 0.0f, 0.2f,	0.5f, 0.2f,
		0.05f, -0.3f, 0.0f,		0.7f, 0.8f, 1.0f,	0.6f, 0.2f,
		0.25f, -0.5f, 0.0f,		0.7f, 0.8f, 0.0f,	0.8f, 0.0f,
		0.45f, -0.3f, 0.0f,		1.0f, 0.4f, 0.4f,	1.0f, 0.2f,
	};

	unsigned int indicesB[] = {
		0, 1, 2,
		2, 3, 4,
		2, 4, 5,
		6, 7, 8,
		6, 9, 8,
		4, 10, 11,
		4, 12, 11,
	};

	const int n = 2;

	float* vertices[] = { verticesA, verticesB };
	int size_vertices[] = { sizeof(verticesA), sizeof(verticesB) };

	unsigned int* indices[] = { indicesA, indicesB };
	int size_indices[] = { sizeof(indicesA), sizeof(indicesB) };

	unsigned int VBO[n], VAO[n], EBO[n];
	glGenVertexArrays(n, VAO);
	glGenBuffers(n, VBO);
	glGenBuffers(n, EBO);

	for (int i = 0; i < n; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, size_vertices[i], vertices[i], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices[i], indices[i], GL_STATIC_DRAW);

		// Position Attrib
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Color Attrib
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture Coords Attrib
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	//Textures
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/texture1.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	width, height, nrChannels;
	data = stbi_load("textures/texture2.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(24.0f / 255.0f, 24.0f / 255.0f, 27.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time_val = glfwGetTime();
		float value = (cos(time_val) / 2.0f) + 0.5f;

		glBindTexture(GL_TEXTURE_2D, texture);
		vf_shader.use();
		vf_shader.setFloat("c", value);

		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);


		glBindTexture(GL_TEXTURE_2D, texture2);
		vf_shader.use();
		vf_shader.setFloat("c", value);

		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);

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
