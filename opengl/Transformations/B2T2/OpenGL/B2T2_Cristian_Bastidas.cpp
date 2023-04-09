#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_s.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Custom functions and methods
float scale_velocity(float velocity);
void speedup(float delta);
float speed = 1;

// settings
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B2T2 - Cristian Bastidas - 1726171042", NULL, NULL);
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

	Shader ourShader("shaders/vertexshader.vs", "shaders/fragmentshader.fs");

	float vertices[] = {
		// Positions			// Texture
		 0.15f,  0.15f, 0.0f,   1.0f, 1.0f,
		 0.15f, -0.15f, 0.0f,   1.0f, 0.0f,
		-0.15f, -0.15f, 0.0f,   0.0f, 0.0f,
		-0.15f,  0.15f, 0.0f,   0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	const float radios[9] = {
		0.0f,	// Sun
		0.28f,	// Mercury
		0.36f,	// Venus
		0.45f,	// Earth
		0.53f,	// Mars
		0.63f,	// Jupiter
		0.75f,	// Saturn
		0.83f,	// Uranus
		0.89f	// Neptune
	};

	const float scales[9] = {
		// Same positions from radios
		0.14f,
		0.2f / 1.5f,
		0.3f / 1.5f,
		0.4f / 1.5f,
		0.3f / 1.5f,
		0.6f / 1.5f,
		0.5f / 1.5f,
		0.3f / 1.5f,
		0.2f / 1.5f
	};

	const int velocities[9] = {
		// Same positions from radios
		0,
		172.404f,
		126.108f,
		107.244f,
		86.868f,
		47.016f,
		34.705f,
		24.516f,
		19.548f
	};

	// Solar system textures
	const char* texture_paths[9] = {
		"textures/sun.png",
		"textures/mercury.png",
		"textures/venus.png",
		"textures/earth.png",
		"textures/mars.png",
		"textures/jupiter.png",
		"textures/saturn.png",
		"textures/uranus.png",
		"textures/neptune.png",
	};

	unsigned int textures[9];
	int width, height, nrChannels;
	for (int i = 0; i < 9; i++) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		// Wraping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load
		unsigned char* data = stbi_load(texture_paths[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);
	}
	ourShader.use();
	ourShader.setInt("Texture", 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		float time = (float)glfwGetTime();

		for (int i = 0; i < 9; i++) {
			glBindVertexArray(VAO);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glm::mat4 transform = glm::mat4(1.0f);

			ourShader.use();
			unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			float scale = i == 0 ? scales[i] * sin(time) + 1.0f : scales[i];
			float vel = scale_velocity(velocities[i]);

			transform = glm::translate(
				transform,
				glm::vec3(
					radios[i] * sin(time * vel),
					radios[i] * cos(time * vel),
					0.0f
				)
			);

			transform = glm::scale(transform, glm::vec3(scale, scale, 0.0f));

			if (i == 0) {
				transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

	// Speed decreases -> F1
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		speedup(-0.05);
		std::cout << "F1: Decreased to " << speed << std::endl;
	}

	// Speed increases -> F2
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		speedup(+0.05);
		std::cout << "F2: Increased to " << speed << std::endl;
	}

	// Speed 0 -> F3
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		speed = 0;
		std::cout << "F3: Planets have been stopped" << std::endl;
	}

	// Speed 1 -> F4
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		speed = 1;
		std::cout << "F4: Speed has been reseted" << std::endl;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void speedup(float delta) {
	if (speed < 0)
		speed = 1;
	speed += delta;
}

float scale_velocity(float velocity) {
	return speed * velocity / 107.244f;
}