/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"

#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"



glm::mat4 viewMatrix;
glm::mat4 perspectiveMatrix;

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}

	glm::vec3 eye = {cameraR, 0.0f, 0.0f};
	glm::vec3 at = {0.0f, 0.0f, 0.0f};
	glm::vec3 up = {0.0f, 0.0f, 1.0f};
	viewMatrix = glm::lookAt(eye, at, up);


	float fov = 45;
	float aspect = 1;
	float near = 0.001;
	float far = 10000;
	perspectiveMatrix = glm::perspective(fov, aspect, near, far);

	glEnable(GL_DEPTH_TEST);
	//glPolygoneMode(GL_FRONT_AND_BACK, GL_LINE);
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<Geometry>& objects) {
	//Clears the screen to a dark grey background
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);

	for (const Geometry& g : objects) {

		//////////////////////////////Texture Code////////////////////////////////
		//Set which texture unit the texture is bound to
		glActiveTexture(GL_TEXTURE0);
		//Bind the texture to GL_TEXTURE0
		glBindTexture(GL_TEXTURE_2D, g.texture.textureID);
		//Get identifier for uniform
		GLuint uniformLocation = glGetUniformLocation(shaderProgram, "imageTexture");
		//Load texture unit number into uniform
		glUniform1i(uniformLocation, 0);
		//////////////////////////////Texture Code/////////////////////////////////

		glm::mat4 modelMatrix = g.translationMatrix * g.rotationMatrix * g.scaleMatrix;

		glm::mat4 transformMatrix = perspectiveMatrix * viewMatrix * modelMatrix;

		GLuint transformLocation = glGetUniformLocation(shaderProgram, "transform");;
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, 	glm::value_ptr(transformMatrix));

		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);
}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}

void RenderingEngine::zoom(double direction){
	float cameraMin = 10;
	float cameraMax = 400;

	if(cameraMin <= cameraR && direction == 1)
		cameraR = cameraR*0.9;

	if(cameraR <= cameraMax && direction == -1)
		cameraR = cameraR*1.1;

	updateEye();
}

void RenderingEngine::rotate(double xChange, double yChange){
	float xChangeRads = xChange*(M_PI/180);
	float yChangeRads = yChange*(M_PI/180);

	cameraTheta += xChangeRads;

	if(0 < cameraPhi+yChangeRads && cameraPhi+yChangeRads < M_PI)
		cameraPhi += yChangeRads;

	updateEye();
}

void RenderingEngine::updateEye(){
	cameraX = cameraR*cos(cameraTheta)*sin(cameraPhi);
	cameraY = cameraR*sin(cameraTheta)*sin(cameraPhi);
	cameraZ = cameraR*cos(cameraPhi);

	glm::vec3 eye = {cameraX, cameraY, cameraZ};
	glm::vec3 at = {0.0f, 0.0f, 0.0f};
	glm::vec3 up = {0.0f, 0.0f, 1.0f};
	viewMatrix = glm::lookAt(eye, at, up);
}

void RenderingEngine::updateRotation(Geometry &g, float t){

}

