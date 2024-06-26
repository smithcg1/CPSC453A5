/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

RenderingEngine* currentEngine;

Program::Program() {
	setupWindow();
}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	renderingEngine = new RenderingEngine();
	currentEngine = renderingEngine;
	scene = new Scene(renderingEngine);


	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1024;
	int height = 1024;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetScrollCallback(window, KeyCallbackScroll);
	glfwSetCursorPosCallback(window, KeyCallbackMouseMove);

	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}



void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		currentEngine->changeFocus("sun");
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		currentEngine->changeFocus("mercury");
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		currentEngine->changeFocus("venus");
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		currentEngine->changeFocus("earth");
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		currentEngine->changeFocus("eMoon");
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		currentEngine->changeFocus("mars");
	}
	if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		currentEngine->changeFocus("jupiter");
	}
	if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
		currentEngine->changeFocus("saturn");
	}
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
		currentEngine->changeFocus("uranus");
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		currentEngine->changeFocus("neptune");
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		currentEngine->changeTime(-1);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		currentEngine->changeTime(0);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		currentEngine->changeTime(1);
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		currentEngine->changeTime(-10);
	}
}


void KeyCallbackScroll(GLFWwindow* window, double xoffset, double yoffset) {
	currentEngine->zoom(yoffset);
}


static void KeyCallbackMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)){
		double xChange = (xpos - oldx);
		double yChange = -(ypos - oldy);

		//std::cout << "X change: " << xChange << "   Y change: " << yChange << std::endl;
		currentEngine->rotate(-xChange, yChange);
	}
/*
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)){
		double xChange = -(xpos - oldx);

		//currentScene->Rotate(xChange);
	}*/
	oldx = xpos;
	oldy = ypos;
}
