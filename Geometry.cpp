/*
 * Geometry.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Geometry.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"
#include <string>

void Geometry::setParent(Geometry *parentPlanet){
	parent = parentPlanet;
}

Geometry::Geometry() : vao(0), vertexBuffer(0), normalBuffer(0), uvBuffer(0), colorBuffer(0) {
	//vectors are initially empty
	//Pointers are initially null
	//Call RenderingEngine::assignBuffers and RenderingEngine::setBufferData to fully initialize the geometry
	//Overload the constructor for more functionality or create subclasses for specific objects
}

Geometry::~Geometry() {

}



void Geometry::createMatrices(float planetR, float rotationPeriod, long int orbitR, float orbitPeriod){

	float size = logn(7.0f, planetR);
	scaleMatrix = glm::transpose(glm::mat4{{size, 0.0f, 0.0f, 0.0f},
											{0.0f, size, 0.0f, 0.0f},
											{0.0f, 0.0f, size, 0.0f},
											{0.0f, 0.0f, 0.0f, 1.0f}});

	theta = (M_PI/rotationPeriod);
	rotationMatrix = glm::transpose(glm::mat4{{cos(theta), -sin(theta), 0.0f, 0.0f},
					   	 	 	 	 	 	 {sin(theta), cos(theta), 	0.0f, 0.0f},
											 {0.0f, 0.0f, 				1.0f, 0.0f},
											 {0.0f, 0.0f, 				0.0f, 1.0f}});


	orbitalTheta = (M_PI/orbitPeriod);
	orbitalDist = logn(3.0f, orbitR);
	if(orbitR == 0)
		orbitalDist = 0;

	translationMatrix = glm::transpose(glm::mat4{{1.0f, 0.0f, 0.0f, orbitalDist},
													{0.0f, 1.0f, 0.0f, 0.0f},
													{0.0f, 0.0f, 1.0f, 0.0f},
													{0.0f, 0.0f, 0.0f, 1.0f}});

}



void Geometry::setTexture(std::string textureName){
	int n = textureName.length();
	char char_array_textureName[n+1];
	strcpy(char_array_textureName, textureName.c_str());
	InitializeTexture(&texture, char_array_textureName, GL_TEXTURE_2D);
}

void Geometry::updateRotation(float t){
	rotationMatrix = glm::transpose(glm::mat4{{cos(t*theta), -sin(t*theta), 0.0f, 0.0f},
					   	 	 	 	 	 	 {sin(t*theta), cos(t*theta), 	0.0f, 0.0f},
											 {0.0f, 0.0f, 				1.0f, 0.0f},
											 {0.0f, 0.0f, 				0.0f, 1.0f}});

}


void Geometry::updateTranslation(float t){
	float x = orbitalDist*cos(t*orbitalTheta);
	float y = orbitalDist*cos(t*orbitalTheta);
	translationMatrix = glm::transpose(glm::mat4{{1.0f, 0.0f, 0.0f, x},
												{0.0f, 1.0f, 0.0f, y},
												{0.0f, 0.0f, 1.0f, 0.0f},
												{0.0f, 0.0f, 0.0f, 1.0f}});
}



float Geometry::logn(float base, long int num){
	return(log(num)/log(base));
}
