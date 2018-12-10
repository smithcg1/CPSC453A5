/*
 * RenderingEngine.h
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#ifndef RENDERINGENGINE_H_
#define RENDERINGENGINE_H_

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class GLFWwindow;

class RenderingEngine {
public:
	RenderingEngine();
	virtual ~RenderingEngine();

	//Renders each object
	void RenderScene(std::vector<Geometry>& objects);

	//Create vao and vbos for objects
	static void assignBuffers(Geometry& geometry);
	static void setBufferData(Geometry& geometry);
	static void deleteBufferData(Geometry& geometry);

	//Ensures that vao and vbos are set up properly
	bool CheckGLErrors();

	float time = 0;
	float timeInc = 0.03;
	float timeFlag = 1;

	float cameraTheta = 0;			//Longitude
	float cameraPhi = (M_PI/2);		//Latitude
	float cameraR = 60;

	float cameraX = 1.2;
	float cameraY = 0;
	float cameraZ = 0;

	void zoom(double direction);
	void rotate(double xChange, double yChange);
	void updateEye();
	void updateRotation(Geometry &g, float t);

	std::string eyeTarget;
	float eyeTargetX = 0.0;
	float eyeTargetY = 0.0;
	float eyeTargetZ = 0.0;
	glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);

	void changeFocus(std::string target);
	void changeTime(int flag);

	//Pointer to the current shader program being used to render
	GLuint shaderProgram;

	MyTexture textureC;
	MyTexture textureDE;
	MyTexture textureSM;
};

#endif /* RENDERINGENGINE_H_ */
