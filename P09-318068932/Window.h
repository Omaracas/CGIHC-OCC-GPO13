#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getMuevePlaneX() { return muevePlaneX; }
	GLfloat getMuevePlaneY() { return muevePlaneY; }
	GLfloat getMueveRoverX() { return mueveRoverX; }
	GLfloat getsatelite1() { return satelite1; }
	GLfloat getsatelite2() { return satelite2; }
	GLfloat getsatelite3() { return satelite3; }
	GLfloat getsatelite4() { return satelite4; }
	GLfloat getsatelite5() { return satelite5; }
	GLfloat getsatelite6() { return satelite6; }
	int getCambiarColor() { return cambiarColor; }
	bool getLuzRover() { return luzRover; }
	bool getLuzHolocron() { return luzHolocron; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat satelite1, satelite2, satelite3, satelite4, satelite5, satelite6;
	GLfloat muevePlaneX, muevePlaneY, mueveRoverX;
	int cambiarColor;
	bool luzHolocron, luzRover;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

