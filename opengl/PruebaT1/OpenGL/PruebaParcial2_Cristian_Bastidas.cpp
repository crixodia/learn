#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

// Pi for cube rotations
constexpr float PI = 3.14159265359;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float speed = 1.0f;	// Camera speed
bool firstMouse = true;

// Timing: time between current frame and last frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "B2T3 - Cristian Bastidas - 172617104", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// Configure global opengl state
	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shaders/vertex.vs", "shaders/fragment.fs");

	// Face block distance
	float x_distance = 0.2965 - 0.0845;
	float y_distance = 0.6735f - 0.4185f;

	// Shift ranges for each face block
	float x_delta[] = { 0.0f, 1.0f, 2.0f, 3.0f, 1.005f, 1.005f };
	float y_delta[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f };


	float vertices[] = {
		// Left 0
		-0.5f,  0.5f,  0.5f,	0.0845f, 0.4185f,
		-0.5f,  0.5f, -0.5f,	0.2965f, 0.4185f,
		-0.5f, -0.5f, -0.5f,	0.2965f, 0.6735f,
		-0.5f, -0.5f,  0.5f,	0.0845f, 0.6735f,

		// Front 1
		-0.5f,  0.5f,  0.5f,	0.0845f, 0.4185f,
		 0.5f,  0.5f,  0.5f,	0.2965f, 0.4185f,
		 0.5f, -0.5f,  0.5f,	0.2965f, 0.6735f,
		-0.5f, -0.5f,  0.5f,	0.0845f, 0.6735f,


		// Right 2
		 0.5f,  0.5f,  0.5f,	0.0845f, 0.4185f,
		 0.5f,  0.5f, -0.5f,	0.2965f, 0.4185f,
		 0.5f, -0.5f, -0.5f,	0.2965f, 0.6735f,
		 0.5f, -0.5f,  0.5f,	0.0845f, 0.6735f,

		 // Back 3
		 -0.5f,  0.5f, -0.5f,	0.0845f, 0.4185f,
		  0.5f,  0.5f, -0.5f,	0.2965f, 0.4185f,
		  0.5f, -0.5f, -0.5f,	0.2965f, 0.6735f,
		 -0.5f, -0.5f, -0.5f,	0.0845f, 0.6735f,

		 // Top 4
		 -0.5f, -0.5f, -0.5f,	0.0845f, 0.4185f,
		  0.5f, -0.5f, -0.5f,	0.2965f, 0.4185f,
		  0.5f, -0.5f,  0.5f,	0.2965f, 0.6735f,
		 -0.5f, -0.5f,  0.5f,	0.0845f, 0.6735f,

		 // Bottom 5
		-0.5f,  0.5f, -0.5f,	0.0845f, 0.4185f,
		 0.5f,  0.5f, -0.5f,	0.2965f, 0.4185f,
		 0.5f,  0.5f,  0.5f,	0.2965f, 0.6735f,
		-0.5f,  0.5f,  0.5f,	0.0845f, 0.6735f,
	};

	float x_distance2 = 0.29 - 0.0868;
	float y_distance2 = 0.5965f - 0.34f;

	// Shift ranges for each face block
	float x_delta2[] = { 1.0f, 2.0f, 3.0f, 0.0f, 2.005f, 2.0005f };
	float y_delta2[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f };

	float vertices2[] = {
		// Left 0
		-0.5f,  0.5f,  0.5f,	0.0868f, 0.34,
		-0.5f,  0.5f, -0.5f,	0.29f, 0.34,
		-0.5f, -0.5f, -0.5f,	0.29f, 0.5965,
		-0.5f, -0.5f,  0.5f,	0.0868f, 0.5965,

		// Front 1
		-0.5f,  0.5f,  0.5f,	0.0868f, 0.34,
		 0.5f,  0.5f,  0.5f,	0.29f, 0.34,
		 0.5f, -0.5f,  0.5f,	0.29f, 0.5965,
		-0.5f, -0.5f,  0.5f,	0.0868f, 0.5965,


		// Right 2
		 0.5f,  0.5f,  0.5f,	0.0868f, 0.34,
		 0.5f,  0.5f, -0.5f,	0.29f, 0.34,
		 0.5f, -0.5f, -0.5f,	0.29f, 0.5965,
		 0.5f, -0.5f,  0.5f,	0.0868f, 0.5965,

		 // Back 3
		 -0.5f,  0.5f, -0.5f,	0.0868f, 0.34,
		  0.5f,  0.5f, -0.5f,	0.29f, 0.34,
		  0.5f, -0.5f, -0.5f,	0.29f, 0.5965,
		 -0.5f, -0.5f, -0.5f,	0.0868f, 0.5965,

		 // Top 4
		 -0.5f, -0.5f, -0.5f,	0.0868f, 0.34,
		  0.5f, -0.5f, -0.5f,	0.29f, 0.34,
		  0.5f, -0.5f,  0.5f,	0.29f, 0.5965,
		 -0.5f, -0.5f,  0.5f,	0.0868f, 0.5965,

		 // Bottom 5
		-0.5f,  0.5f, -0.5f,	0.0868f, 0.34,
		 0.5f,  0.5f, -0.5f,	0.29f, 0.34,
		 0.5f,  0.5f,  0.5f,	0.29f, 0.5965,
		-0.5f,  0.5f,  0.5f,	0.0868f, 0.5965,
	};

	// Updates face texture position based on its shift range
	int k = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			vertices[5 * k + 3] += x_distance * x_delta[i];
			vertices[5 * k + 4] += y_distance * y_delta[i];

			vertices2[5 * k + 3] += x_distance2 * x_delta2[i];
			vertices2[5 * k + 4] += y_distance2 * y_delta2[i];
			k++;
		}
	}

	unsigned int indices[] = {
		// Left
		0,1,2,
		2,3,0,

		// Front
		4,5,6,
		6,7,4,

		// Right
		8,9,10,
		10,11,8,

		// Back
		12,13,14,
		14,15,12,

		// Top
		16,17,18,
		18,19,16,

		// Bottom
		20,21,22,
		22,23,20
	};

	unsigned int VBO[2], VAO[2], EBO[2];

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Array of used textures
	unsigned int textures[4];
	const char* texture_paths[4] = {
		"textures/emerald.jpg",
		"textures/gold.jpg",
		"textures/Texture6.jpg",
		"textures/Texture6.jpg",
	};

	// Loading textures
	int width, height, nrChannels;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < 4; i++) {
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);
	}

	ourShader.use();
	ourShader.setInt("texture1", 0);

	glm::vec3 positions[4]{
		glm::vec3(0.0f, 0.0f, -4.0f),
		glm::vec3(1.0f, -2.0f, -2.0f),
		glm::vec3(-1.0f, 2.0f, -2.0f),
		glm::vec3(-1.0f, -2.0f, -2.0f),
	};

	glm::vec3 rotations[2]{
		glm::vec3(1.0f, 0.0f, 0.0f),	// X
		glm::vec3(0.0f, 0.0f, 1.0f),	// Z
	};



	while (!glfwWindowShouldClose(window))
	{
		// Frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// Camera/View
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		for (int i = 0; i < 4; i++) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			if (i < 2) {
				glBindVertexArray(VAO[0]);
			}
			else {
				glBindVertexArray(VAO[1]);
			}

			glBindTexture(GL_TEXTURE_2D, textures[i]);	// Changes to emeral textures
			glm::mat4 model = glm::mat4(1.0f);

			switch (i)
			{
			case 1:
				model = glm::translate(model, positions[i] + 3.0f * glm::vec3(sin(currentFrame), cos(currentFrame), 0.0f));
			case 2:
				model = glm::translate(model, positions[i] + 3.0f * glm::vec3(sin(currentFrame), 0.0f, sin(currentFrame)));
				break;
			default:
				model = glm::translate(model, positions[i]);
				model = glm::rotate(model, currentFrame, rotations[i%2]);
				break;
			}

			ourShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, speed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, speed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, speed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, speed * deltaTime);

	// Triangles mesh
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Press left control key to increase camera speed like minecraft :)
	speed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? 4.0f : 1.0f;

	//If I want to stay in ground level (xz plane)
	//camera.Position.y = 0.0f;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}