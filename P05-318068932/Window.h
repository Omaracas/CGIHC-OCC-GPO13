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
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getholocron1() { return holocron1; }
	GLfloat getholocron2() { return holocron2; }
	GLfloat getholocron3() { return holocron3; }
	GLfloat getholocron4() { return holocron4; }
	GLfloat getholocron5() { return holocron5; }
	GLfloat getholocron6() { return holocron6; }
	GLfloat getholocron7() { return holocron7; }
	GLfloat getholocron8() { return holocron8; }
	GLfloat getsatelite1() { return satelite1; }
	GLfloat getsatelite2() { return satelite2; }
	GLfloat getsatelite3() { return satelite3; }
	GLfloat getsatelite4() { return satelite4; }
	GLfloat getsatelite5() { return satelite5; }
	GLfloat getsatelite6() { return satelite6; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz;
	GLfloat satelite1, satelite2, satelite3, satelite4, satelite5, satelite6;
	GLfloat holocron1, holocron2, holocron3, holocron4, holocron5, holocron6, holocron7, holocron8;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

