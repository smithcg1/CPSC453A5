/*
 * Geometry.h
 *	Class for storing geometry
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"
#include <string>

class Geometry {
public:
	Geometry();
	virtual ~Geometry();
	std::string name;

	//Data structures for storing vertices, normals colors and uvs
	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> uvs;

	//Pointers to the vao and vbos associated with the geometry
	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint colorBuffer;

	//Draw mode for how OpenGL interprets primitives
	GLuint drawMode;

	MyTexture texture;

	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;

	Geometry *parent = 0;

	float theta = 0;
	float orbitalDist = 0;
	float orbitalTheta = 0;
	float orbitalPhi = 0;
	float orbitalIncl = 90;
	float orbitalPeriod = 0;
	float axialTilt = 90;

	void updateTranslation(float t, glm::mat4 translationMatrix);

	void createMatrices(float tilt, float inclination, float planetR, float rotationPeriod, long int orbitR, float orbitPeriod);
	void setTexture(std::string textureName);
	void updateRotation(float t);
	void updateTranslation(float t);

	void setParent(Geometry *parentPlanet);

	float logn(float base, float num);
	float logn(float base, long int num);
};

#endif /* GEOMETRY_H_ */
