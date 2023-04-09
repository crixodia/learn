
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Ancho de la ventana
const unsigned int SCR_WIDTH = 800;
// Alto de la ventana
const unsigned int SCR_HEIGHT = 600;

// Se escribe el vertex shader en GLSL
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // Cada vértice tiene una coordenada 3D (vec3)
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Perspective division = 1.0
"}\0";

// Se escribe el fragment shader en GLSL
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(215.0f/255.0f, 148.0f/255.0f, 0.0f, 1.0f);\n"    // El color del fragment shader
"}\n\0";

int main()
{
	// Inicializa la librería GWlf
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	/*
	* Crea una ventana de tamaño 800x600 de título "First Triangle" de OpenGL en modo ventana sin compartir recursos
	* a otra ventana
	* (width:int, height:int, title:string, monitor:a monitor object, share: a window object)
	*/
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First Triangle", NULL, NULL);
	if (window == NULL)
	{
		// Mensaje de error
		std::cout << "Failed to create GLFW window" << std::endl;
		/*
		* Destruye todas las ventanas restantes y libera todos los recursos usados
		*/
		glfwTerminate();
		return -1;  //Finaliza el programa con error
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Se crea el shader object
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Se adjunta el código del shader al shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Se compila el sahder
	glCompileShader(vertexShader);

	// Este bloque de código comprueba si la compilación fue exitosa
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	// Se crea el objeto fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Se adjunta el código del fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Se compila el fragment shader
	glCompileShader(fragmentShader);

	// Este bloque de código comprueba se la compilación fue exitosa
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}


	// Se crea el shader program
	int shaderProgram = glCreateProgram();
	// Se adjunta los shader creados anteiormente al object program
	glAttachShader(shaderProgram, vertexShader);    // El vertex shader
	glAttachShader(shaderProgram, fragmentShader);  // El fragment shader
	// Enlace los shaders creados con object program
	glLinkProgram(shaderProgram);

	// Este bloque de codigo comprueba si el enlace fue exitoso
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			<< infoLog << std::endl;
	}

	// Ya no necesitaremos el vertex shader y fragment shader dado a que ya están enlazados
	// al shader program. Por loc ual se borran para liberar recursos.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Un arreglo de números flotantes normalizados en base a la región visible de OpenGL
	// Aqué se toma la coordenada z como 0 dado a que se requiere un triángulo en 2D

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f,  // right
		0.0f, 0.5f, 0.0f    // top
	};

	unsigned int VBO, VAO; //(Vertex buffer object, Vertex array object)
	// Se generan los vertex array objects
	glGenVertexArrays(1, &VAO);

	// Se generan buffers con ID único (Vertex buffer objects)
	glGenBuffers(1, &VBO);

	// Se une los vertex array objects
	glBindVertexArray(VAO);

	// Se unen lo buffers creados a GL_ARRAY_BUFFER (Vertex buffer objects)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Se copian los vértices definidos previamente en la memoria del buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Se establecen los punteros de atributos de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// render loop
	// -----------
	// Retorna el valor de la señal de cerrar de una ventana
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		// Se define el color de fondo
		glClearColor(24.0f / 255.0f, 24.0f / 255.0f, 27.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activamos el program object creado previamente
		glUseProgram(shaderProgram);

		// Se generan buffers con ID único (Vertex buffer objects)
		glBindVertexArray(VAO);

		// Toma un tipo primitivo de OpenGL  que se quiere dibujar, el índice de inicio, cuántos vértices
		// queremos dibujar
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Intercambia los buffers de una ventana específica (fron y back)
		glfwSwapBuffers(window);
		// Procesa solo los eventos que están en la cola de eventos
		glfwPollEvents();
	}

	//Borramos lo que no necesitamos
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// LIbera los recursos
	glfwTerminate();
	return 0;   // El programa finalizó correctamente
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
