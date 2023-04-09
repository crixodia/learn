#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /*  
    * Inicializa la librer�a GLFw
    * retorna falso en caso de que exista un error
    */
    if (!glfwInit())
        return -1;  //Finaliza el programa con error

    /*
    * Crea una ventana de tama�o 640x480 de t�tulo "Hello World" de OpenGL en modo ventana sin compartir recursos 
    * a otra ventana
    * (width:int, height:int, title:string, monitor:a monitor object, share: a window object)
    */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        /*
        * Destruye todas las ventanas restantes y libera todos los recursos usados
        */
        glfwTerminate();
        return -1;  // Finaliza el programa con error
    }

    // Se pasa el estado del anterior contexto al nuevo contexto
    glfwMakeContextCurrent(window);

    // Retorna el valor de la se�al de cerrar de una ventana
    while (!glfwWindowShouldClose(window))
    {
        // Limpia el b�ffer (Los disponibles para la escritura de colores)
        glClear(GL_COLOR_BUFFER_BIT);

        // Intercambia los buffers de una ventana espec�fica (fron y back)
        glfwSwapBuffers(window);

        // Procesa solo los eventos que est�n en la cola de eventos
        glfwPollEvents();
    }

    /*
    * Destruye todas las ventanas restantes y libera todos los recursos usados
    */
    glfwTerminate();
    return 0;   //El programa se termina sin errores
}