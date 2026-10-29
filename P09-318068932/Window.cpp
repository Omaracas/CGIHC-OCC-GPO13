#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevePlaneX = 0.0f;
	muevePlaneY = 0.0f;
	mueveRoverX = 0.0f;
	cambiarColor = 0;
	luzHolocron = true;
	luzRover = true;

	satelite1 = 0.0f;
	satelite2 = 0.0f;
	satelite3 = 0.0f;
	satelite4 = 0.0f;
	satelite5 = 0.0f;
	satelite6 = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 09-1: Animacion basica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_X)
	{
		theWindow->satelite1 += 7.0;
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->satelite2 += 7.0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->satelite3 += 7.0;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->satelite4 += 7.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->satelite5 += 7.0;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->satelite6 += 7.0;
	}

	/*if (key == GLFW_KEY_H)
	{
		theWindow-> muevePlaneX -= 1.0;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow-> muevePlaneX += 1.0;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow-> muevePlaneY += 1.0;
	}
	if (key == GLFW_KEY_G && theWindow-> muevePlaneY>-4.0)
	{
		theWindow-> muevePlaneY -= 1.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow-> mueveRoverX -= 1.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow-> mueveRoverX += 1.0;
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		theWindow-> cambiarColor +=1;
		if (theWindow->cambiarColor >= 6) {
			theWindow->cambiarColor = 0;
		}
	}

	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		theWindow->luzHolocron = !theWindow->luzHolocron;
	}*/

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		theWindow->luzRover = !theWindow->luzRover;
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
