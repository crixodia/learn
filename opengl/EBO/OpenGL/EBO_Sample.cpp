//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 800;
//
//
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//
//
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(0.1f, 0.2f, 0.3f, 1.0f);\n"
//"}\n\0";
//
//int main()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
//#endif
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EBO", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//	glCompileShader(vertexShader);
//
//	int success;
//	char infoLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//			<< infoLog << std::endl;
//	}
//
//	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(fragmentShader);
//
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
//			<< infoLog << std::endl;
//	}
//
//	int shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//
//	glLinkProgram(shaderProgram);
//
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
//			<< infoLog << std::endl;
//	}
//
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//	float vertices[] = {
//		-0.25f, -0.1f, 0.0f,
//		-0.45f, -0.3f, 0.0f,
//		-0.25f, -0.5f, 0.0f,
//		-0.55f, 0.6f, 0.0f,
//		0.25f, 0.6f, 0.0f,
//		-0.15f, 0.2f, 0.0f,
//		0.25f, 0.2f, 0.0f,
//		-0.05f, -0.1f, 0.0f,
//		0.25f, -0.1f, 0.0f,
//		-0.55f, 0.2f, 0.0f,
//		-0.15f, -0.2f, 0.0f,
//		-0.05f, -0.3f, 0.0f,
//		0.05f, -0.3f, 0.0f,
//		0.25f, -0.5f, 0.0f,
//		0.45f, -0.3f, 0.0f,
//		0.55f, 0.2f, 0.0f,
//	};
//
//	unsigned int indices[] = {
//		0, 1, 2,
//		3, 4, 5,
//		6, 7, 8,
//		9, 10, 6,
//		0, 11, 2,
//		8, 12, 13,
//		8, 14, 13,
//		6, 8, 15,
//		3, 9, 5,
//	};
//
//
//
//	unsigned int VBO, VAO, EBO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	// RENDER LOOP
//	while (!glfwWindowShouldClose(window))
//	{
//
//		processInput(window);
//		glClearColor(24.0f / 255.0f, 24.0f / 255.0f, 27.0f / 255.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//
//		glUseProgram(shaderProgram);
//
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, 9 * sizeof(indices), GL_UNSIGNED_INT, 0);
//
//
//		glBindVertexArray(0);
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	//Borramos lo que no necesitamos
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
//
//	// Libera los recursos
//	glfwTerminate();
//	return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	else
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}