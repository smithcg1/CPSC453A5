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



void Geometry::createMatrices(float tilt, float inclination, float planetR, float rotationPeriod, long int orbitR, float orbitPeriod){
	orbitalPeriod = orbitPeriod;
	orbitalIncl = inclination;
	axialTilt = tilt;

	float size = logn(637.0f, planetR);
	std::cout << "Planet: " << name << "    Size: " << size << std::endl;
	if(planetR == 500)
		size = 500;
	if(name == "eMoon"){
		size = 0.3375;
	}
	scaleMatrix = glm::transpose(glm::mat4{{size, 0.0f, 0.0f, 0.0f},
											{0.0f, size, 0.0f, 0.0f},
											{0.0f, 0.0f, size, 0.0f},
											{0.0f, 0.0f, 0.0f, 1.0f}});



	theta = (M_PI/rotationPeriod);
	if(rotationPeriod == 0)
		theta = 0;
	rotationMatrix = glm::transpose(glm::mat4{{cos(theta), -sin(theta), 0.0f, 0.0f},
					   	 	 	 	 	 	 {sin(theta), cos(theta), 	0.0f, 0.0f},
											 {0.0f, 0.0f, 				1.0f, 0.0f},
											 {0.0f, 0.0f, 				0.0f, 1.0f}});


	orbitalTheta = (2*M_PI/orbitPeriod);
	//rbitalDist = logn(149600000.0f, orbitR) * 30;

	orbitalDist = glm::pow(orbitR/100000, 0.5);

	if(name == "eMoon")
		orbitalDist = logn(149600000.0f, orbitR)*4;

	std::cout << "Planet: " << name << "    orbitalDist: " << orbitalDist << std::endl;
	if(orbitR == 0){
		orbitalDist = 0;
		orbitalTheta = 0;
	}

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
	float tilt = axialTilt*(M_PI/180);
	glm::mat4 axialTiltMatrix = glm::transpose(glm::mat4{{1.0f, 	0.0f , 			0.0f, 			0.0f},
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 {0.0f, 	cos(tilt), 	-sin(tilt), 	0.0f},
														 {0.0f, 	sin(tilt), 	cos(tilt), 	0.0f},
														{0.0f, 		0.0f, 			0.0f, 			1.0f}});

	glm::mat4 spinMatrix = glm::transpose(glm::mat4{{cos(t*theta), -sin(t*theta), 0.0f, 0.0f},
					   	 	 	 	 	 	 {sin(t*theta), cos(t*theta), 	0.0f, 0.0f},
											 {0.0f, 0.0f, 				1.0f, 0.0f},
											 {0.0f, 0.0f, 				0.0f, 1.0f}});


	rotationMatrix = axialTiltMatrix*spinMatrix;
}


void Geometry::updateTranslation(float t){
	float x = orbitalDist*cos(t*orbitalTheta);
	float y = orbitalDist*sin(t*orbitalTheta);

	float inclination = orbitalIncl*(M_PI/180);
	glm::mat4 orbitMatrix = glm::transpose(glm::mat4{{1.0f, 0.0f, 0.0f, x},
													{0.0f, 1.0f, 0.0f, y},
													{0.0f, 0.0f, 1.0f, 0.0f},
													{0.0f, 0.0f, 0.0f, 1.0f}});



	if(parent != NULL){
		float z = sin(t*orbitalTheta)*0.246;
		glm::mat4 fudge = glm::transpose(glm::mat4{{0.0f, 0.0f, 0.0f, 0.0f},
												{0.0f, 0.0f, 0.0f, 0.0f},
												{0.0f, 0.0f, 0.0f, z},
												{0.0f, 0.0f, 0.0f, 0.0f}});

		orbitMatrix = orbitMatrix * parent->translationMatrix +fudge;
	}


	glm::mat4 inclinationMatrix = glm::transpose(glm::mat4{{1.0f, 	0.0f , 			0.0f, 			0.0f},
															{0.0f, 	cos(inclination), 	-sin(inclination), 	0.0f},
															{0.0f, 	sin(inclination), 	cos(inclination), 	0.0f},
															{0.0f, 		0.0f, 			0.0f, 			1.0f}});

	translationMatrix = inclinationMatrix*orbitMatrix;
}


float Geometry::logn(float base, float num){
	return(log(num)/log(base));
}

float Geometry::logn(float base, long int num){
	return(log(num)/log(base));
}



/*

orbitalTheta = t*(2*M_PI/orbitalPeriod);
orbitalPhi = cos(orbitalTheta)*((90-orbitalIncl)*(M_PI/180));


float x = orbitalDist*cos(orbitalTheta)*sin(orbitalPhi);;
float y = orbitalDist*sin(orbitalTheta)*sin(orbitalPhi);
float z = orbitalDist*cos(orbitalPhi);


float inclination = orbitalIncl*(M_PI/180);
glm::mat4 orbitMatrix = glm::transpose(glm::mat4{{1.0f, 0.0f, 0.0f, x},
												{0.0f, 1.0f, 0.0f, y},
												{0.0f, 0.0f, 1.0f, z},
												{0.0f, 0.0f, 0.0f, 1.0f}});



if(parent != NULL){
	orbitMatrix = orbitMatrix * parent->translationMatrix;
}

/*
float inclineTime = sin((t*2*M_PI)/orbitalPeriod)*inclination;

glm::mat4 inclinationMatrix = glm::transpose(glm::mat4{{1.0f, 	0.0f , 			0.0f, 			0.0f},
														{0.0f, 	cos(inclination), 	-sin(inclination), 	0.0f},
														{0.0f, 	sin(inclination), 	cos(inclination), 	0.0f},
														{0.0f, 		0.0f, 			0.0f, 			1.0f}});

//translationMatrix = orbitMatrix; //inclinationMatrix*/
