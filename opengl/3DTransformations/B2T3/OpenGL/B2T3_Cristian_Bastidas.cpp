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
void load_positions();

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

// Size of positions for each texture
const int d1 = 285, d2 = 85, d3 = 117;
const int d4 = 2, d5 = 5, d6 = 74;
const int d7 = 106, d8 = 7, d9 = 2;
const int d10 = 4, d11 = 16, d12 = 3;
const int d13 = 1;

const int sizes[] = { d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13 };

// Block positions for each texture
glm::vec3 WOOD[d1];
glm::vec3 WHITE_WOOL[d2];
glm::vec3 WHITE_WOOD[d3];
glm::vec3 EMERALD[d4];
glm::vec3 GOLD[d5];
glm::vec3 NETHER[d6];
glm::vec3 DARK_WOOD[d7];
glm::vec3 CHEST[d8];
glm::vec3 HAY_BALE[d9];
glm::vec3 FURNACE[d10];
glm::vec3 TNT[d11];
glm::vec3 GRAY_WOOL[d12];
glm::vec3 CRAFTING_TABLE[d13];

// Array of each block positions to make it easy during render loop
glm::vec3* positions[] = {
	WOOD, WHITE_WOOL, WHITE_WOOD,
	EMERALD, GOLD, NETHER,
	DARK_WOOD, CHEST, HAY_BALE,
	FURNACE, TNT, GRAY_WOOL,
	CRAFTING_TABLE
};

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

	// Updates face texture position based on its shift range
	int k = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			vertices[5 * k + 3] += x_distance * x_delta[i];
			vertices[5 * k + 4] += y_distance * y_delta[i];
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

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Array of used textures
	unsigned int textures[13];
	const char* texture_paths[13] = {
		"textures/wood.png",
		"textures/white_wool.png",
		"textures/white_wood.png",
		"textures/emerald.png",
		"textures/gold.png",
		"textures/nether.png",
		"textures/dark_wood.png",
		"textures/chest.png",
		"textures/hay_bale.png",
		"textures/furnace.png",
		"textures/tnt.png",
		"textures/gray_wool.png",
		"textures/crafting_table.png",
	};

	// Loading textures
	int width, height, nrChannels;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < 13; i++) {
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
	ourShader.setInt("texture1", 0);
	load_positions();

	while (!glfwWindowShouldClose(window))
	{
		// Frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		glClearColor(0.0f, 0.48f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// Camera/View
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		glBindVertexArray(VAO);
		// Drawing blocks on their positions for each texture

		for (int k = 0; k < 13; k++) {	// Loops textures
			glBindTexture(GL_TEXTURE_2D, textures[k]);
			for (unsigned int i = 0; i < sizes[k]; i++)
			{
				for (int j = 0; j < 2; j++) {	// Taking advantage of symmetry
					glm::vec3 pos = positions[k][i];
					int simet = j == 1 ? -1 : 1;	// Mirror related to x coordinate

					pos.x = simet * positions[k][i].x;

					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, pos);	// Moving block on its position

					// Rotates chests because its front face matters
					float angle = 0.0f;
					if (k == 7) {
						if (positions[k][i].z == -12.0f)	// 180 degrees
							angle = PI;
						else if ((positions[k][i].z == -26.0f || positions[k][i].z == -24.0f))
							angle = -simet * PI / 2;	// 90 degrees or -90 when it is mirrored
						model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));	// rotates chest
					}
					ourShader.setMat4("model", model);
					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				}
			}
		}

		// Emerald rotating
		glBindTexture(GL_TEXTURE_2D, textures[3]);	// Changes to emeral textures
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 14.0f, -16.0f));
		model = glm::scale(
			model,
			(sin(currentFrame) * glm::vec3(0.2f)) + glm::vec3(0.5f)
		);
		model = glm::rotate(model, currentFrame, glm::vec3(1.0f, -1.0f, 1.0f));
		ourShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// 2 Gold blocks rotating and moving
		glBindTexture(GL_TEXTURE_2D, textures[4]);	// Changes to gold texture
		glm::vec3 init = glm::vec3(0.0f, 14.0f, -16.0f);
		float radio = 1.5f;
		float s = radio * sin(currentFrame);
		float c = radio * cos(currentFrame);

		model = glm::mat4(1.0f);
		model = glm::translate(model, init + glm::vec3(s, s, c));
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::rotate(model, currentFrame, glm::vec3(-1.0f));
		ourShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, init + glm::vec3(s, c, c));
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::rotate(model, currentFrame, glm::vec3(1.0f));
		ourShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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

// Load all coordinates for each block by texture
void load_positions() {
	glm::vec3 wood[] = {
		glm::vec3(0.0f, 13.0f, -16.0f),
		glm::vec3(0.0f, 12.0f, -16.0f),
		glm::vec3(0.0f, 11.0f, -16.0f),
		glm::vec3(0.0f, 10.0f, -16.0f),
		glm::vec3(0.0f, 9.0f, -16.0f),
		glm::vec3(0.0f, 8.0f, -16.0f),
		glm::vec3(0.0f, 7.0f, -16.0f),
		glm::vec3(0.0f, 6.0f, -16.0f),
		glm::vec3(0.0f, 5.0f, -16.0f),
		glm::vec3(0.0f, 4.0f, -16.0f),
		glm::vec3(0.0f, 3.0f, -16.0f),
		glm::vec3(0.0f, 2.0f, -16.0f),
		glm::vec3(0.0f, 1.0f, -16.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -4.0f),
		glm::vec3(0.0f, 0.0f, -16.0f),
		glm::vec3(0.0f, -1.0f, -3.0f),
		glm::vec3(0.0f, -1.0f, -4.0f),
		glm::vec3(0.0f, -1.0f, -16.0f),
		glm::vec3(0.0f, -2.0f, -5.0f),
		glm::vec3(0.0f, -2.0f, -6.0f),
		glm::vec3(0.0f, -2.0f, -7.0f),
		glm::vec3(0.0f, -2.0f, -16.0f),
		glm::vec3(0.0f, -2.0f, -31.0f),
		glm::vec3(0.0f, -3.0f, -6.0f),
		glm::vec3(0.0f, -3.0f, -7.0f),
		glm::vec3(0.0f, -3.0f, -10.0f),
		glm::vec3(0.0f, -3.0f, -30.0f),
		glm::vec3(0.0f, -4.0f, -8.0f),
		glm::vec3(0.0f, -4.0f, -9.0f),
		glm::vec3(0.0f, -4.0f, -30.0f),
		glm::vec3(0.0f, -5.0f, -10.0f),
		glm::vec3(0.0f, -5.0f, -11.0f),
		glm::vec3(0.0f, -5.0f, -30.0f),
		glm::vec3(0.0f, -6.0f, -12.0f),
		glm::vec3(0.0f, -6.0f, -29.0f),
		glm::vec3(0.0f, -7.0f, -13.0f),
		glm::vec3(0.0f, -7.0f, -14.0f),
		glm::vec3(0.0f, -7.0f, -15.0f),
		glm::vec3(0.0f, -7.0f, -16.0f),
		glm::vec3(0.0f, -7.0f, -17.0f),
		glm::vec3(0.0f, -7.0f, -18.0f),
		glm::vec3(0.0f, -7.0f, -19.0f),
		glm::vec3(0.0f, -7.0f, -20.0f),
		glm::vec3(0.0f, -7.0f, -21.0f),
		glm::vec3(0.0f, -7.0f, -22.0f),
		glm::vec3(0.0f, -7.0f, -23.0f),
		glm::vec3(0.0f, -7.0f, -24.0f),
		glm::vec3(0.0f, -7.0f, -25.0f),
		glm::vec3(0.0f, -7.0f, -26.0f),
		glm::vec3(0.0f, -7.0f, -27.0f),
		glm::vec3(0.0f, -7.0f, -28.0f),
		glm::vec3(1.0f, 8.0f, -16.0f),
		glm::vec3(1.0f, 1.0f, -16.0f),
		glm::vec3(1.0f, -1.0f, -4.0f),
		glm::vec3(1.0f, -2.0f, -5.0f),
		glm::vec3(1.0f, -2.0f, -6.0f),
		glm::vec3(1.0f, -2.0f, -7.0f),
		glm::vec3(1.0f, -2.0f, -31.0f),
		glm::vec3(1.0f, -3.0f, -6.0f),
		glm::vec3(1.0f, -3.0f, -7.0f),
		glm::vec3(1.0f, -3.0f, -30.0f),
		glm::vec3(1.0f, -4.0f, -8.0f),
		glm::vec3(1.0f, -4.0f, -9.0f),
		glm::vec3(1.0f, -4.0f, -10.0f),
		glm::vec3(1.0f, -4.0f, -30.0f),
		glm::vec3(1.0f, -5.0f, -10.0f),
		glm::vec3(1.0f, -5.0f, -11.0f),
		glm::vec3(1.0f, -5.0f, -30.0f),
		glm::vec3(1.0f, -6.0f, -12.0f),
		glm::vec3(1.0f, -6.0f, -29.0f),
		glm::vec3(1.0f, -7.0f, -13.0f),
		glm::vec3(1.0f, -7.0f, -14.0f),
		glm::vec3(1.0f, -7.0f, -15.0f),
		glm::vec3(1.0f, -7.0f, -16.0f),
		glm::vec3(1.0f, -7.0f, -17.0f),
		glm::vec3(1.0f, -7.0f, -18.0f),
		glm::vec3(1.0f, -7.0f, -19.0f),
		glm::vec3(1.0f, -7.0f, -20.0f),
		glm::vec3(1.0f, -7.0f, -21.0f),
		glm::vec3(1.0f, -7.0f, -22.0f),
		glm::vec3(1.0f, -7.0f, -23.0f),
		glm::vec3(1.0f, -7.0f, -24.0f),
		glm::vec3(1.0f, -7.0f, -25.0f),
		glm::vec3(1.0f, -7.0f, -26.0f),
		glm::vec3(1.0f, -7.0f, -27.0f),
		glm::vec3(1.0f, -7.0f, -28.0f),
		glm::vec3(2.0f, 8.0f, -16.0f),
		glm::vec3(2.0f, 1.0f, -16.0f),
		glm::vec3(2.0f, -1.0f, -4.0f),
		glm::vec3(2.0f, -1.0f, -5.0f),
		glm::vec3(2.0f, -1.0f, -6.0f),
		glm::vec3(2.0f, -2.0f, -6.0f),
		glm::vec3(2.0f, -2.0f, -7.0f),
		glm::vec3(2.0f, -2.0f, -8.0f),
		glm::vec3(2.0f, -2.0f, -31.0f),
		glm::vec3(2.0f, -3.0f, -7.0f),
		glm::vec3(2.0f, -3.0f, -8.0f),
		glm::vec3(2.0f, -3.0f, -30.0f),
		glm::vec3(2.0f, -4.0f, -9.0f),
		glm::vec3(2.0f, -4.0f, -30.0f),
		glm::vec3(2.0f, -5.0f, -10.0f),
		glm::vec3(2.0f, -5.0f, -11.0f),
		glm::vec3(2.0f, -5.0f, -30.0f),
		glm::vec3(2.0f, -6.0f, -12.0f),
		glm::vec3(2.0f, -6.0f, -29.0f),
		glm::vec3(2.0f, -7.0f, -13.0f),
		glm::vec3(2.0f, -7.0f, -14.0f),
		glm::vec3(2.0f, -7.0f, -15.0f),
		glm::vec3(2.0f, -7.0f, -16.0f),
		glm::vec3(2.0f, -7.0f, -17.0f),
		glm::vec3(2.0f, -7.0f, -18.0f),
		glm::vec3(2.0f, -7.0f, -19.0f),
		glm::vec3(2.0f, -7.0f, -20.0f),
		glm::vec3(2.0f, -7.0f, -21.0f),
		glm::vec3(2.0f, -7.0f, -22.0f),
		glm::vec3(2.0f, -7.0f, -23.0f),
		glm::vec3(2.0f, -7.0f, -24.0f),
		glm::vec3(2.0f, -7.0f, -25.0f),
		glm::vec3(2.0f, -7.0f, -26.0f),
		glm::vec3(2.0f, -7.0f, -27.0f),
		glm::vec3(2.0f, -7.0f, -28.0f),
		glm::vec3(3.0f, 8.0f, -16.0f),
		glm::vec3(3.0f, 1.0f, -16.0f),
		glm::vec3(3.0f, -2.0f, -8.0f),
		glm::vec3(3.0f, -2.0f, -9.0f),
		glm::vec3(3.0f, -2.0f, -31.0f),
		glm::vec3(3.0f, -3.0f, -8.0f),
		glm::vec3(3.0f, -3.0f, -30.0f),
		glm::vec3(3.0f, -4.0f, -9.0f),
		glm::vec3(3.0f, -4.0f, -30.0f),
		glm::vec3(3.0f, -5.0f, -10.0f),
		glm::vec3(3.0f, -5.0f, -11.0f),
		glm::vec3(3.0f, -5.0f, -30.0f),
		glm::vec3(3.0f, -6.0f, -12.0f),
		glm::vec3(3.0f, -6.0f, -29.0f),
		glm::vec3(3.0f, -7.0f, -13.0f),
		glm::vec3(3.0f, -7.0f, -14.0f),
		glm::vec3(3.0f, -7.0f, -15.0f),
		glm::vec3(3.0f, -7.0f, -16.0f),
		glm::vec3(3.0f, -7.0f, -17.0f),
		glm::vec3(3.0f, -7.0f, -18.0f),
		glm::vec3(3.0f, -7.0f, -19.0f),
		glm::vec3(3.0f, -7.0f, -20.0f),
		glm::vec3(3.0f, -7.0f, -21.0f),
		glm::vec3(3.0f, -7.0f, -22.0f),
		glm::vec3(3.0f, -7.0f, -23.0f),
		glm::vec3(3.0f, -7.0f, -24.0f),
		glm::vec3(3.0f, -7.0f, -25.0f),
		glm::vec3(3.0f, -7.0f, -26.0f),
		glm::vec3(3.0f, -7.0f, -27.0f),
		glm::vec3(3.0f, -7.0f, -28.0f),
		glm::vec3(4.0f, 8.0f, -16.0f),
		glm::vec3(4.0f, 1.0f, -16.0f),
		glm::vec3(4.0f, -2.0f, -9.0f),
		glm::vec3(4.0f, -2.0f, -10.0f),
		glm::vec3(4.0f, -2.0f, -31.0f),
		glm::vec3(4.0f, -3.0f, -9.0f),
		glm::vec3(4.0f, -3.0f, -10.0f),
		glm::vec3(4.0f, -3.0f, -29.0f),
		glm::vec3(4.0f, -3.0f, -30.0f),
		glm::vec3(4.0f, -4.0f, -10.0f),
		glm::vec3(4.0f, -4.0f, -11.0f),
		glm::vec3(4.0f, -4.0f, -29.0f),
		glm::vec3(4.0f, -4.0f, -30.0f),
		glm::vec3(4.0f, -5.0f, -11.0f),
		glm::vec3(4.0f, -5.0f, -29.0f),
		glm::vec3(4.0f, -5.0f, -30.0f),
		glm::vec3(4.0f, -6.0f, -12.0f),
		glm::vec3(4.0f, -6.0f, -29.0f),
		glm::vec3(4.0f, -7.0f, -13.0f),
		glm::vec3(4.0f, -7.0f, -14.0f),
		glm::vec3(4.0f, -7.0f, -15.0f),
		glm::vec3(4.0f, -7.0f, -16.0f),
		glm::vec3(4.0f, -7.0f, -17.0f),
		glm::vec3(4.0f, -7.0f, -18.0f),
		glm::vec3(4.0f, -7.0f, -19.0f),
		glm::vec3(4.0f, -7.0f, -20.0f),
		glm::vec3(4.0f, -7.0f, -21.0f),
		glm::vec3(4.0f, -7.0f, -22.0f),
		glm::vec3(4.0f, -7.0f, -23.0f),
		glm::vec3(4.0f, -7.0f, -24.0f),
		glm::vec3(4.0f, -7.0f, -25.0f),
		glm::vec3(4.0f, -7.0f, -26.0f),
		glm::vec3(4.0f, -7.0f, -27.0f),
		glm::vec3(4.0f, -7.0f, -28.0f),
		glm::vec3(5.0f, 8.0f, -16.0f),
		glm::vec3(5.0f, -2.0f, -10.0f),
		glm::vec3(5.0f, -2.0f, -11.0f),
		glm::vec3(5.0f, -2.0f, -12.0f),
		glm::vec3(5.0f, -2.0f, -13.0f),
		glm::vec3(5.0f, -2.0f, -28.0f),
		glm::vec3(5.0f, -2.0f, -29.0f),
		glm::vec3(5.0f, -2.0f, -30.0f),
		glm::vec3(5.0f, -3.0f, -10.0f),
		glm::vec3(5.0f, -3.0f, -11.0f),
		glm::vec3(5.0f, -3.0f, -12.0f),
		glm::vec3(5.0f, -3.0f, -13.0f),
		glm::vec3(5.0f, -3.0f, -28.0f),
		glm::vec3(5.0f, -4.0f, -12.0f),
		glm::vec3(5.0f, -4.0f, -28.0f),
		glm::vec3(5.0f, -5.0f, -12.0f),
		glm::vec3(5.0f, -5.0f, -13.0f),
		glm::vec3(5.0f, -5.0f, -28.0f),
		glm::vec3(5.0f, -6.0f, -13.0f),
		glm::vec3(5.0f, -6.0f, -14.0f),
		glm::vec3(5.0f, -6.0f, -15.0f),
		glm::vec3(5.0f, -6.0f, -16.0f),
		glm::vec3(5.0f, -6.0f, -17.0f),
		glm::vec3(5.0f, -6.0f, -18.0f),
		glm::vec3(5.0f, -6.0f, -19.0f),
		glm::vec3(5.0f, -6.0f, -20.0f),
		glm::vec3(5.0f, -6.0f, -21.0f),
		glm::vec3(5.0f, -6.0f, -22.0f),
		glm::vec3(5.0f, -6.0f, -23.0f),
		glm::vec3(5.0f, -6.0f, -24.0f),
		glm::vec3(5.0f, -6.0f, -25.0f),
		glm::vec3(5.0f, -6.0f, -26.0f),
		glm::vec3(5.0f, -6.0f, -27.0f),
		glm::vec3(5.0f, -6.0f, -28.0f),
		glm::vec3(6.0f, -2.0f, -13.0f),
		glm::vec3(6.0f, -2.0f, -14.0f),
		glm::vec3(6.0f, -2.0f, -15.0f),
		glm::vec3(6.0f, -2.0f, -16.0f),
		glm::vec3(6.0f, -2.0f, -17.0f),
		glm::vec3(6.0f, -2.0f, -18.0f),
		glm::vec3(6.0f, -2.0f, -19.0f),
		glm::vec3(6.0f, -2.0f, -20.0f),
		glm::vec3(6.0f, -2.0f, -21.0f),
		glm::vec3(6.0f, -2.0f, -22.0f),
		glm::vec3(6.0f, -2.0f, -23.0f),
		glm::vec3(6.0f, -2.0f, -24.0f),
		glm::vec3(6.0f, -2.0f, -25.0f),
		glm::vec3(6.0f, -2.0f, -26.0f),
		glm::vec3(6.0f, -2.0f, -27.0f),
		glm::vec3(6.0f, -2.0f, -28.0f),
		glm::vec3(6.0f, -3.0f, -13.0f),
		glm::vec3(6.0f, -3.0f, -14.0f),
		glm::vec3(6.0f, -3.0f, -15.0f),
		glm::vec3(6.0f, -3.0f, -16.0f),
		glm::vec3(6.0f, -3.0f, -17.0f),
		glm::vec3(6.0f, -3.0f, -18.0f),
		glm::vec3(6.0f, -3.0f, -19.0f),
		glm::vec3(6.0f, -3.0f, -20.0f),
		glm::vec3(6.0f, -3.0f, -21.0f),
		glm::vec3(6.0f, -3.0f, -22.0f),
		glm::vec3(6.0f, -3.0f, -23.0f),
		glm::vec3(6.0f, -3.0f, -24.0f),
		glm::vec3(6.0f, -3.0f, -25.0f),
		glm::vec3(6.0f, -3.0f, -26.0f),
		glm::vec3(6.0f, -3.0f, -27.0f),
		glm::vec3(6.0f, -3.0f, -28.0f),
		glm::vec3(6.0f, -4.0f, -13.0f),
		glm::vec3(6.0f, -4.0f, -14.0f),
		glm::vec3(6.0f, -4.0f, -15.0f),
		glm::vec3(6.0f, -4.0f, -16.0f),
		glm::vec3(6.0f, -4.0f, -17.0f),
		glm::vec3(6.0f, -4.0f, -18.0f),
		glm::vec3(6.0f, -4.0f, -19.0f),
		glm::vec3(6.0f, -4.0f, -20.0f),
		glm::vec3(6.0f, -4.0f, -21.0f),
		glm::vec3(6.0f, -4.0f, -22.0f),
		glm::vec3(6.0f, -4.0f, -23.0f),
		glm::vec3(6.0f, -4.0f, -24.0f),
		glm::vec3(6.0f, -4.0f, -25.0f),
		glm::vec3(6.0f, -4.0f, -26.0f),
		glm::vec3(6.0f, -4.0f, -27.0f),
		glm::vec3(6.0f, -4.0f, -28.0f),
		glm::vec3(6.0f, -5.0f, -14.0f),
		glm::vec3(6.0f, -5.0f, -15.0f),
		glm::vec3(6.0f, -5.0f, -16.0f),
		glm::vec3(6.0f, -5.0f, -17.0f),
		glm::vec3(6.0f, -5.0f, -18.0f),
		glm::vec3(6.0f, -5.0f, -19.0f),
		glm::vec3(6.0f, -5.0f, -20.0f),
		glm::vec3(6.0f, -5.0f, -21.0f),
		glm::vec3(6.0f, -5.0f, -22.0f),
		glm::vec3(6.0f, -5.0f, -23.0f),
		glm::vec3(6.0f, -5.0f, -24.0f),
		glm::vec3(6.0f, -5.0f, -25.0f),
		glm::vec3(6.0f, -5.0f, -26.0f),
		glm::vec3(6.0f, -5.0f, -27.0f),
		glm::vec3(6.0f, -5.0f, -28.0f),
	};
	glm::vec3 white_wool[] = {
		glm::vec3(0.0f, 8.0f, -11.0f),
		glm::vec3(0.0f, 7.0f, -11.0f),
		glm::vec3(0.0f, 6.0f, -11.0f),
		glm::vec3(0.0f, 5.0f, -11.0f),
		glm::vec3(0.0f, 3.0f, -11.0f),
		glm::vec3(0.0f, 3.0f, -12.0f),
		glm::vec3(0.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 1.0f, -13.0f),
		glm::vec3(0.0f, 1.0f, -15.0f),
		glm::vec3(0.0f, 0.0f, -14.0f),
		glm::vec3(1.0f, 8.0f, -11.0f),
		glm::vec3(1.0f, 7.0f, -11.0f),
		glm::vec3(1.0f, 5.0f, -11.0f),
		glm::vec3(1.0f, 3.0f, -11.0f),
		glm::vec3(1.0f, 3.0f, -12.0f),
		glm::vec3(1.0f, 2.0f, -12.0f),
		glm::vec3(1.0f, 1.0f, -13.0f),
		glm::vec3(1.0f, 1.0f, -15.0f),
		glm::vec3(1.0f, 0.0f, -14.0f),
		glm::vec3(2.0f, 8.0f, -11.0f),
		glm::vec3(2.0f, 7.0f, -11.0f),
		glm::vec3(2.0f, 6.0f, -11.0f),
		glm::vec3(2.0f, 5.0f, -11.0f),
		glm::vec3(2.0f, 4.0f, -11.0f),
		glm::vec3(2.0f, 3.0f, -11.0f),
		glm::vec3(2.0f, 3.0f, -12.0f),
		glm::vec3(2.0f, 2.0f, -12.0f),
		glm::vec3(2.0f, 1.0f, -13.0f),
		glm::vec3(2.0f, 1.0f, -15.0f),
		glm::vec3(2.0f, 0.0f, -14.0f),
		glm::vec3(3.0f, 8.0f, -12.0f),
		glm::vec3(3.0f, 8.0f, -13.0f),
		glm::vec3(3.0f, 7.0f, -11.0f),
		glm::vec3(3.0f, 6.0f, -11.0f),
		glm::vec3(3.0f, 5.0f, -11.0f),
		glm::vec3(3.0f, 4.0f, -11.0f),
		glm::vec3(3.0f, 3.0f, -12.0f),
		glm::vec3(3.0f, 2.0f, -12.0f),
		glm::vec3(3.0f, 1.0f, -13.0f),
		glm::vec3(3.0f, 1.0f, -15.0f),
		glm::vec3(3.0f, 0.0f, -14.0f),
		glm::vec3(4.0f, 8.0f, -12.0f),
		glm::vec3(4.0f, 8.0f, -13.0f),
		glm::vec3(4.0f, 7.0f, -12.0f),
		glm::vec3(4.0f, 6.0f, -12.0f),
		glm::vec3(4.0f, 5.0f, -12.0f),
		glm::vec3(4.0f, 4.0f, -12.0f),
		glm::vec3(4.0f, 3.0f, -12.0f),
		glm::vec3(4.0f, 2.0f, -12.0f),
		glm::vec3(4.0f, 1.0f, -13.0f),
		glm::vec3(4.0f, 0.0f, -13.0f),
		glm::vec3(4.0f, 0.0f, -14.0f),
		glm::vec3(4.0f, 0.0f, -15.0f),
		glm::vec3(5.0f, 8.0f, -13.0f),
		glm::vec3(5.0f, 7.0f, -13.0f),
		glm::vec3(5.0f, 6.0f, -13.0f),
		glm::vec3(5.0f, 5.0f, -13.0f),
		glm::vec3(5.0f, 4.0f, -13.0f),
		glm::vec3(5.0f, 3.0f, -13.0f),
		glm::vec3(5.0f, 2.0f, -13.0f),
		glm::vec3(5.0f, 1.0f, -14.0f),
		glm::vec3(5.0f, 1.0f, -15.0f),
		glm::vec3(5.0f, 1.0f, -16.0f),
		glm::vec3(6.0f, 8.0f, -14.0f),
		glm::vec3(6.0f, 8.0f, -15.0f),
		glm::vec3(6.0f, 8.0f, -16.0f),
		glm::vec3(6.0f, 7.0f, -14.0f),
		glm::vec3(6.0f, 7.0f, -15.0f),
		glm::vec3(6.0f, 7.0f, -16.0f),
		glm::vec3(6.0f, 6.0f, -14.0f),
		glm::vec3(6.0f, 6.0f, -15.0f),
		glm::vec3(6.0f, 6.0f, -16.0f),
		glm::vec3(6.0f, 5.0f, -14.0f),
		glm::vec3(6.0f, 5.0f, -15.0f),
		glm::vec3(6.0f, 5.0f, -16.0f),
		glm::vec3(6.0f, 4.0f, -14.0f),
		glm::vec3(6.0f, 4.0f, -15.0f),
		glm::vec3(6.0f, 4.0f, -16.0f),
		glm::vec3(6.0f, 3.0f, -14.0f),
		glm::vec3(6.0f, 3.0f, -15.0f),
		glm::vec3(6.0f, 3.0f, -16.0f),
		glm::vec3(6.0f, 2.0f, -14.0f),
		glm::vec3(6.0f, 2.0f, -15.0f),
		glm::vec3(6.0f, 2.0f, -16.0f),
		glm::vec3(6.0f, 1.0f, -15.0f),
	};
	glm::vec3 white_wood[] = {
		glm::vec3(0.0f, -3.0f, -8.0f),
		glm::vec3(0.0f, -3.0f, -9.0f),
		glm::vec3(0.0f, -3.0f, -11.0f),
		glm::vec3(0.0f, -3.0f, -12.0f),
		glm::vec3(0.0f, -3.0f, -13.0f),
		glm::vec3(0.0f, -3.0f, -14.0f),
		glm::vec3(0.0f, -3.0f, -15.0f),
		glm::vec3(0.0f, -3.0f, -16.0f),
		glm::vec3(0.0f, -3.0f, -17.0f),
		glm::vec3(0.0f, -3.0f, -21.0f),
		glm::vec3(0.0f, -3.0f, -22.0f),
		glm::vec3(0.0f, -3.0f, -23.0f),
		glm::vec3(0.0f, -3.0f, -24.0f),
		glm::vec3(0.0f, -3.0f, -25.0f),
		glm::vec3(0.0f, -3.0f, -26.0f),
		glm::vec3(0.0f, -3.0f, -27.0f),
		glm::vec3(0.0f, -3.0f, -28.0f),
		glm::vec3(0.0f, -3.0f, -29.0f),
		glm::vec3(0.0f, -4.0f, -20.0f),
		glm::vec3(0.0f, -5.0f, -19.0f),
		glm::vec3(0.0f, -6.0f, -18.0f),
		glm::vec3(1.0f, -3.0f, -8.0f),
		glm::vec3(1.0f, -3.0f, -9.0f),
		glm::vec3(1.0f, -3.0f, -10.0f),
		glm::vec3(1.0f, -3.0f, -11.0f),
		glm::vec3(1.0f, -3.0f, -12.0f),
		glm::vec3(1.0f, -3.0f, -13.0f),
		glm::vec3(1.0f, -3.0f, -14.0f),
		glm::vec3(1.0f, -3.0f, -15.0f),
		glm::vec3(1.0f, -3.0f, -16.0f),
		glm::vec3(1.0f, -3.0f, -17.0f),
		glm::vec3(1.0f, -3.0f, -21.0f),
		glm::vec3(1.0f, -3.0f, -22.0f),
		glm::vec3(1.0f, -3.0f, -23.0f),
		glm::vec3(1.0f, -3.0f, -24.0f),
		glm::vec3(1.0f, -3.0f, -25.0f),
		glm::vec3(1.0f, -3.0f, -26.0f),
		glm::vec3(1.0f, -3.0f, -27.0f),
		glm::vec3(1.0f, -3.0f, -28.0f),
		glm::vec3(1.0f, -3.0f, -29.0f),
		glm::vec3(1.0f, -4.0f, -20.0f),
		glm::vec3(1.0f, -5.0f, -19.0f),
		glm::vec3(1.0f, -6.0f, -18.0f),
		glm::vec3(2.0f, -3.0f, -9.0f),
		glm::vec3(2.0f, -3.0f, -10.0f),
		glm::vec3(2.0f, -3.0f, -11.0f),
		glm::vec3(2.0f, -3.0f, -12.0f),
		glm::vec3(2.0f, -3.0f, -13.0f),
		glm::vec3(2.0f, -3.0f, -14.0f),
		glm::vec3(2.0f, -3.0f, -15.0f),
		glm::vec3(2.0f, -3.0f, -16.0f),
		glm::vec3(2.0f, -3.0f, -17.0f),
		glm::vec3(2.0f, -3.0f, -18.0f),
		glm::vec3(2.0f, -3.0f, -19.0f),
		glm::vec3(2.0f, -3.0f, -20.0f),
		glm::vec3(2.0f, -3.0f, -21.0f),
		glm::vec3(2.0f, -3.0f, -22.0f),
		glm::vec3(2.0f, -3.0f, -23.0f),
		glm::vec3(2.0f, -3.0f, -24.0f),
		glm::vec3(2.0f, -3.0f, -25.0f),
		glm::vec3(2.0f, -3.0f, -26.0f),
		glm::vec3(2.0f, -3.0f, -27.0f),
		glm::vec3(2.0f, -3.0f, -28.0f),
		glm::vec3(2.0f, -3.0f, -29.0f),
		glm::vec3(3.0f, -3.0f, -9.0f),
		glm::vec3(3.0f, -3.0f, -10.0f),
		glm::vec3(3.0f, -3.0f, -11.0f),
		glm::vec3(3.0f, -3.0f, -12.0f),
		glm::vec3(3.0f, -3.0f, -13.0f),
		glm::vec3(3.0f, -3.0f, -14.0f),
		glm::vec3(3.0f, -3.0f, -15.0f),
		glm::vec3(3.0f, -3.0f, -16.0f),
		glm::vec3(3.0f, -3.0f, -17.0f),
		glm::vec3(3.0f, -3.0f, -18.0f),
		glm::vec3(3.0f, -3.0f, -19.0f),
		glm::vec3(3.0f, -3.0f, -20.0f),
		glm::vec3(3.0f, -3.0f, -21.0f),
		glm::vec3(3.0f, -3.0f, -22.0f),
		glm::vec3(3.0f, -3.0f, -23.0f),
		glm::vec3(3.0f, -3.0f, -24.0f),
		glm::vec3(3.0f, -3.0f, -25.0f),
		glm::vec3(3.0f, -3.0f, -26.0f),
		glm::vec3(3.0f, -3.0f, -27.0f),
		glm::vec3(3.0f, -3.0f, -28.0f),
		glm::vec3(3.0f, -3.0f, -29.0f),
		glm::vec3(4.0f, -3.0f, -11.0f),
		glm::vec3(4.0f, -3.0f, -12.0f),
		glm::vec3(4.0f, -3.0f, -13.0f),
		glm::vec3(4.0f, -3.0f, -14.0f),
		glm::vec3(4.0f, -3.0f, -15.0f),
		glm::vec3(4.0f, -3.0f, -16.0f),
		glm::vec3(4.0f, -3.0f, -17.0f),
		glm::vec3(4.0f, -3.0f, -18.0f),
		glm::vec3(4.0f, -3.0f, -19.0f),
		glm::vec3(4.0f, -3.0f, -20.0f),
		glm::vec3(4.0f, -3.0f, -21.0f),
		glm::vec3(4.0f, -3.0f, -22.0f),
		glm::vec3(4.0f, -3.0f, -23.0f),
		glm::vec3(4.0f, -3.0f, -24.0f),
		glm::vec3(4.0f, -3.0f, -25.0f),
		glm::vec3(4.0f, -3.0f, -26.0f),
		glm::vec3(4.0f, -3.0f, -27.0f),
		glm::vec3(4.0f, -3.0f, -28.0f),
		glm::vec3(5.0f, -3.0f, -14.0f),
		glm::vec3(5.0f, -3.0f, -15.0f),
		glm::vec3(5.0f, -3.0f, -16.0f),
		glm::vec3(5.0f, -3.0f, -17.0f),
		glm::vec3(5.0f, -3.0f, -18.0f),
		glm::vec3(5.0f, -3.0f, -19.0f),
		glm::vec3(5.0f, -3.0f, -20.0f),
		glm::vec3(5.0f, -3.0f, -21.0f),
		glm::vec3(5.0f, -3.0f, -22.0f),
		glm::vec3(5.0f, -3.0f, -23.0f),
		glm::vec3(5.0f, -3.0f, -24.0f),
		glm::vec3(5.0f, -3.0f, -25.0f),
		glm::vec3(5.0f, -3.0f, -26.0f),
		glm::vec3(5.0f, -3.0f, -27.0f),
	};
	glm::vec3 emerald[] = {
		glm::vec3(4.0f, -6.0f, -28.0f),
		glm::vec3(5.0f, -5.0f, -27.0f),
	};
	glm::vec3 gold[] = {
		glm::vec3(2.0f, -2.0f, -29.0f),
		glm::vec3(4.0f, -5.0f, -28.0f),
		glm::vec3(4.0f, -6.0f, -26.0f),
		glm::vec3(4.0f, -6.0f, -27.0f),
		glm::vec3(5.0f, -5.0f, -26.0f),
	};
	glm::vec3 nether[] = {
		glm::vec3(0.0f, 8.0f, -26.0f),
		glm::vec3(0.0f, 8.0f, -27.0f),
		glm::vec3(0.0f, 8.0f, -28.0f),
		glm::vec3(0.0f, 8.0f, -29.0f),
		glm::vec3(0.0f, 8.0f, -30.0f),
		glm::vec3(0.0f, 8.0f, -31.0f),
		glm::vec3(0.0f, 6.0f, -22.0f),
		glm::vec3(0.0f, 6.0f, -23.0f),
		glm::vec3(0.0f, 6.0f, -24.0f),
		glm::vec3(0.0f, 6.0f, -25.0f),
		glm::vec3(0.0f, 6.0f, -26.0f),
		glm::vec3(1.0f, 7.0f, -26.0f),
		glm::vec3(1.0f, 7.0f, -27.0f),
		glm::vec3(1.0f, 7.0f, -28.0f),
		glm::vec3(1.0f, 7.0f, -29.0f),
		glm::vec3(1.0f, 7.0f, -30.0f),
		glm::vec3(1.0f, 7.0f, -31.0f),
		glm::vec3(1.0f, 5.0f, -22.0f),
		glm::vec3(1.0f, 5.0f, -23.0f),
		glm::vec3(1.0f, 5.0f, -24.0f),
		glm::vec3(1.0f, 5.0f, -25.0f),
		glm::vec3(1.0f, 5.0f, -26.0f),
		glm::vec3(2.0f, 6.0f, -26.0f),
		glm::vec3(2.0f, 6.0f, -27.0f),
		glm::vec3(2.0f, 6.0f, -28.0f),
		glm::vec3(2.0f, 6.0f, -29.0f),
		glm::vec3(2.0f, 6.0f, -30.0f),
		glm::vec3(2.0f, 6.0f, -31.0f),
		glm::vec3(2.0f, 4.0f, -22.0f),
		glm::vec3(2.0f, 4.0f, -23.0f),
		glm::vec3(2.0f, 4.0f, -24.0f),
		glm::vec3(2.0f, 4.0f, -25.0f),
		glm::vec3(2.0f, 4.0f, -26.0f),
		glm::vec3(3.0f, 5.0f, -26.0f),
		glm::vec3(3.0f, 5.0f, -27.0f),
		glm::vec3(3.0f, 5.0f, -28.0f),
		glm::vec3(3.0f, 5.0f, -29.0f),
		glm::vec3(3.0f, 5.0f, -30.0f),
		glm::vec3(3.0f, 5.0f, -31.0f),
		glm::vec3(3.0f, 3.0f, -22.0f),
		glm::vec3(3.0f, 3.0f, -23.0f),
		glm::vec3(3.0f, 3.0f, -24.0f),
		glm::vec3(3.0f, 3.0f, -25.0f),
		glm::vec3(3.0f, 3.0f, -26.0f),
		glm::vec3(4.0f, 4.0f, -26.0f),
		glm::vec3(4.0f, 4.0f, -27.0f),
		glm::vec3(4.0f, 4.0f, -28.0f),
		glm::vec3(4.0f, 4.0f, -29.0f),
		glm::vec3(4.0f, 4.0f, -30.0f),
		glm::vec3(4.0f, 4.0f, -31.0f),
		glm::vec3(4.0f, 3.0f, -30.0f),
		glm::vec3(4.0f, 2.0f, -22.0f),
		glm::vec3(4.0f, 2.0f, -23.0f),
		glm::vec3(4.0f, 2.0f, -24.0f),
		glm::vec3(4.0f, 2.0f, -25.0f),
		glm::vec3(4.0f, 2.0f, -26.0f),
		glm::vec3(4.0f, 2.0f, -27.0f),
		glm::vec3(4.0f, 2.0f, -28.0f),
		glm::vec3(4.0f, 2.0f, -29.0f),
		glm::vec3(4.0f, 2.0f, -30.0f),
		glm::vec3(4.0f, 1.0f, -30.0f),
		glm::vec3(4.0f, 0.0f, -30.0f),
		glm::vec3(4.0f, -1.0f, -30.0f),
		glm::vec3(5.0f, 3.0f, -26.0f),
		glm::vec3(5.0f, 3.0f, -27.0f),
		glm::vec3(5.0f, 3.0f, -28.0f),
		glm::vec3(5.0f, 3.0f, -29.0f),
		glm::vec3(5.0f, 3.0f, -30.0f),
		glm::vec3(5.0f, 3.0f, -31.0f),
		glm::vec3(5.0f, 1.0f, -22.0f),
		glm::vec3(5.0f, 1.0f, -23.0f),
		glm::vec3(5.0f, 1.0f, -24.0f),
		glm::vec3(5.0f, 1.0f, -25.0f),
		glm::vec3(5.0f, 1.0f, -26.0f),
	};
	glm::vec3 dark_wood[] = {
		glm::vec3(0.0f, 11.0f, -14.0f),
		glm::vec3(0.0f, 11.0f, -18.0f),
		glm::vec3(0.0f, 10.0f, -15.0f),
		glm::vec3(0.0f, 10.0f, -17.0f),
		glm::vec3(0.0f, 7.0f, -27.0f),
		glm::vec3(0.0f, 7.0f, -28.0f),
		glm::vec3(0.0f, 7.0f, -29.0f),
		glm::vec3(0.0f, 7.0f, -30.0f),
		glm::vec3(0.0f, 6.0f, -30.0f),
		glm::vec3(0.0f, 5.0f, -23.0f),
		glm::vec3(0.0f, 5.0f, -30.0f),
		glm::vec3(0.0f, 4.0f, -23.0f),
		glm::vec3(0.0f, 4.0f, -30.0f),
		glm::vec3(0.0f, 3.0f, -23.0f),
		glm::vec3(0.0f, 3.0f, -30.0f),
		glm::vec3(0.0f, 2.0f, -23.0f),
		glm::vec3(0.0f, 2.0f, -30.0f),
		glm::vec3(0.0f, 1.0f, -23.0f),
		glm::vec3(0.0f, 1.0f, -30.0f),
		glm::vec3(0.0f, 0.0f, -10.0f),
		glm::vec3(0.0f, 0.0f, -30.0f),
		glm::vec3(0.0f, -1.0f, -10.0f),
		glm::vec3(0.0f, -1.0f, -30.0f),
		glm::vec3(0.0f, -2.0f, -10.0f),
		glm::vec3(0.0f, -2.0f, -30.0f),
		glm::vec3(1.0f, 11.0f, -14.0f),
		glm::vec3(1.0f, 11.0f, -18.0f),
		glm::vec3(1.0f, 10.0f, -15.0f),
		glm::vec3(1.0f, 10.0f, -16.0f),
		glm::vec3(1.0f, 10.0f, -17.0f),
		glm::vec3(1.0f, 6.0f, -27.0f),
		glm::vec3(1.0f, 6.0f, -30.0f),
		glm::vec3(1.0f, 5.0f, -30.0f),
		glm::vec3(1.0f, 4.0f, -23.0f),
		glm::vec3(1.0f, 4.0f, -30.0f),
		glm::vec3(1.0f, 3.0f, -23.0f),
		glm::vec3(1.0f, 3.0f, -30.0f),
		glm::vec3(1.0f, 2.0f, -23.0f),
		glm::vec3(1.0f, 2.0f, -30.0f),
		glm::vec3(1.0f, 1.0f, -23.0f),
		glm::vec3(1.0f, 1.0f, -30.0f),
		glm::vec3(1.0f, 0.0f, -30.0f),
		glm::vec3(1.0f, -1.0f, -10.0f),
		glm::vec3(1.0f, -1.0f, -30.0f),
		glm::vec3(1.0f, -2.0f, -30.0f),
		glm::vec3(2.0f, 11.0f, -15.0f),
		glm::vec3(2.0f, 11.0f, -16.0f),
		glm::vec3(2.0f, 11.0f, -17.0f),
		glm::vec3(2.0f, 5.0f, -27.0f),
		glm::vec3(2.0f, 5.0f, -30.0f),
		glm::vec3(2.0f, 4.0f, -30.0f),
		glm::vec3(2.0f, 3.0f, -23.0f),
		glm::vec3(2.0f, 3.0f, -30.0f),
		glm::vec3(2.0f, 2.0f, -23.0f),
		glm::vec3(2.0f, 2.0f, -30.0f),
		glm::vec3(2.0f, 1.0f, -23.0f),
		glm::vec3(2.0f, 1.0f, -30.0f),
		glm::vec3(2.0f, 0.0f, -23.0f),
		glm::vec3(2.0f, 0.0f, -30.0f),
		glm::vec3(2.0f, -1.0f, -23.0f),
		glm::vec3(2.0f, -1.0f, -30.0f),
		glm::vec3(2.0f, -2.0f, -23.0f),
		glm::vec3(2.0f, -2.0f, -30.0f),
		glm::vec3(3.0f, 4.0f, -27.0f),
		glm::vec3(3.0f, 4.0f, -30.0f),
		glm::vec3(3.0f, 3.0f, -27.0f),
		glm::vec3(3.0f, 3.0f, -30.0f),
		glm::vec3(3.0f, 2.0f, -23.0f),
		glm::vec3(3.0f, 2.0f, -30.0f),
		glm::vec3(3.0f, 1.0f, -23.0f),
		glm::vec3(3.0f, 1.0f, -30.0f),
		glm::vec3(3.0f, 0.0f, -23.0f),
		glm::vec3(3.0f, 0.0f, -30.0f),
		glm::vec3(3.0f, -1.0f, -23.0f),
		glm::vec3(3.0f, -1.0f, -30.0f),
		glm::vec3(3.0f, -2.0f, -23.0f),
		glm::vec3(3.0f, -2.0f, -30.0f),
		glm::vec3(4.0f, 3.0f, -27.0f),
		glm::vec3(4.0f, 3.0f, -28.0f),
		glm::vec3(4.0f, 3.0f, -29.0f),
		glm::vec3(4.0f, 1.0f, -23.0f),
		glm::vec3(4.0f, 1.0f, -27.0f),
		glm::vec3(4.0f, 1.0f, -28.0f),
		glm::vec3(4.0f, 1.0f, -29.0f),
		glm::vec3(4.0f, 0.0f, -23.0f),
		glm::vec3(4.0f, 0.0f, -24.0f),
		glm::vec3(4.0f, 0.0f, -25.0f),
		glm::vec3(4.0f, 0.0f, -26.0f),
		glm::vec3(4.0f, 0.0f, -27.0f),
		glm::vec3(4.0f, 0.0f, -28.0f),
		glm::vec3(4.0f, 0.0f, -29.0f),
		glm::vec3(4.0f, -1.0f, -23.0f),
		glm::vec3(4.0f, -1.0f, -24.0f),
		glm::vec3(4.0f, -1.0f, -25.0f),
		glm::vec3(4.0f, -1.0f, -26.0f),
		glm::vec3(4.0f, -1.0f, -27.0f),
		glm::vec3(4.0f, -1.0f, -28.0f),
		glm::vec3(4.0f, -1.0f, -29.0f),
		glm::vec3(4.0f, -2.0f, -23.0f),
		glm::vec3(4.0f, -2.0f, -24.0f),
		glm::vec3(4.0f, -2.0f, -25.0f),
		glm::vec3(4.0f, -2.0f, -26.0f),
		glm::vec3(4.0f, -2.0f, -27.0f),
		glm::vec3(4.0f, -2.0f, -28.0f),
		glm::vec3(4.0f, -2.0f, -29.0f),
		glm::vec3(4.0f, -2.0f, -30.0f),
	};
	glm::vec3 chest[] = {
		glm::vec3(0.0f, -5.0f, -12.0f),
		glm::vec3(1.0f, -2.0f, -29.0f),
		glm::vec3(2.0f, -5.0f, -12.0f),
		glm::vec3(3.0f, -2.0f, -29.0f),
		glm::vec3(4.0f, 1.0f, -24.0f),
		glm::vec3(4.0f, 1.0f, -26.0f),
		glm::vec3(4.0f, -5.0f, -12.0f),
	};
	glm::vec3 hay_bale[] = {
		glm::vec3(3.0f, 3.0f, -28.0f),
		glm::vec3(3.0f, 3.0f, -29.0f),
	};
	glm::vec3 furnace[] = {
		glm::vec3(0.0f, -5.0f, -29.0f),
		glm::vec3(1.0f, -5.0f, -29.0f),
		glm::vec3(2.0f, -5.0f, -29.0f),
		glm::vec3(3.0f, -5.0f, -29.0f),
	};
	glm::vec3 tnt[] = {
		glm::vec3(0.0f, -4.0f, -21.0f),
		glm::vec3(0.0f, -5.0f, -20.0f),
		glm::vec3(0.0f, -5.0f, -21.0f),
		glm::vec3(0.0f, -6.0f, -19.0f),
		glm::vec3(0.0f, -6.0f, -20.0f),
		glm::vec3(0.0f, -6.0f, -21.0f),
		glm::vec3(1.0f, -4.0f, -21.0f),
		glm::vec3(1.0f, -5.0f, -20.0f),
		glm::vec3(1.0f, -5.0f, -21.0f),
		glm::vec3(1.0f, -6.0f, -19.0f),
		glm::vec3(1.0f, -6.0f, -20.0f),
		glm::vec3(1.0f, -6.0f, -21.0f),
		glm::vec3(3.0f, 0.0f, -24.0f),
		glm::vec3(3.0f, -1.0f, -24.0f),
		glm::vec3(3.0f, -2.0f, -24.0f),
		glm::vec3(3.0f, -2.0f, -25.0f),
	};
	glm::vec3 gray_wool[] = {
		glm::vec3(0.0f, 4.0f, -11.0f),
		glm::vec3(1.0f, 6.0f, -11.0f),
		glm::vec3(1.0f, 4.0f, -11.0f),
	};
	glm::vec3 crafting_table[] = {
		glm::vec3(0.0f, -2.0f, -29.0f),
	};

	glm::vec3* p[] = {
		wood, white_wool, white_wood,
		emerald, gold, nether,
		dark_wood, chest, hay_bale,
		furnace, tnt, gray_wool,
		crafting_table
	};

	for (int k = 0; k < 13; k++) {
		for (int i = 0; i < sizes[k]; i++)
			positions[k][i] = p[k][i];
	}
}
