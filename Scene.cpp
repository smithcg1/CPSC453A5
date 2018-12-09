/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */

#include "Scene.h"

#include <iostream>

#include "RenderingEngine.h"
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include <stdio.h>
#include <string>

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	generateSceneGraph();
	//	Geometry object, Texture, axialTilt, planetR, rotationPeriod, orbitR, orbitPeriod
	//createPlanet(space, "MySpace.obj", "8k_stars.jpg", 			0.0,	500,		0,		0,			0);
	createPlanet(sun, 	"MySun.obj", "8k_sun.jpg", 				0.0,	695508, 	24.47, 	0, 			0);
	createPlanet(earth, "MyEarth.obj", "8k_earth_daymap.jpg", 	23.44,	6371, 		1, 		149600000, 	600); // 149600000, 	20
	createPlanet(eMoon, "MyEMoon.obj", "8k_moon.jpg", 			1.54,	1737, 		27.3, 	384400, 	1); //384400, 	27
	//createPlanet(mars, "MyMars.obj", "8k_mars.jpg", 			25.19,	3390, 		24.66, 	227900000, 	687);
}

Scene::~Scene() {

}

void Scene::displayScene() {
	renderer->RenderScene(objects);
}

void Scene::createPlanet(Geometry &planet, std::string fileName, std::string textureName, float axialTilt, float planetR, float rotationPeriod, long int orbitR, float orbitPeriod){
	readPlanet(planet, fileName);
	planet.createMatrices(axialTilt, planetR, rotationPeriod, orbitR, orbitPeriod);
	planet.setTexture(textureName);
	addPlanet(planet);
}


Geometry Scene::readPlanet(Geometry &planet, std::string fileName){
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> vertexTextures;
	std::vector<glm::vec3> vertexNormals;

	char str [80];
	float f1, f2, f3;
	int i1,i2,i3,i4,i5,i6,i7,i8,i9;


	int n = fileName.length();
	char char_array_filename[n+1];
	strcpy(char_array_filename, fileName.c_str());
	FILE * eFile = fopen (char_array_filename,"r");
	if(eFile == NULL){printf ("No such file exists");}


	char remove [80];
	for(int i = 0; i < 10; i++){
		fscanf (eFile, "%s\n", &remove);
		//std::cout << remove << std::endl;
	}

	while(fscanf (eFile, "v %f %f %f\n", &f1, &f2, &f3)){
		//vertices.push_back(glm::vec3(f1,f2,f3));
		vertices.push_back(glm::vec3(f1,-f3,f2));
		//std::cout << "v1:" << f1 << "  v2:" << f2 << "  v3:" << f3 << std::endl;
	}

	fscanf (eFile, "t %f %f\n", &f1, &f2);
	vertexTextures.push_back(glm::vec2(f1,f2));
	//std::cout << "vt1:" << f1 << "  vt2:" << f2 << std::endl;

	while(fscanf (eFile, "vt %f %f\n", &f1, &f2)){
		vertexTextures.push_back(glm::vec2(f1,f2));
		//std::cout << "vt1:" << f1 << "  vt2:" << f2 << std::endl;
	}

	fscanf (eFile, "n %f %f %f\n", &f1, &f2, &f3);
	vertexNormals.push_back(glm::vec3(f1,-f3,f2));
	//std::cout << "vn1:" << f1 << "  vn2:" << f2 << "  vn3:" << f3 << std::endl;


	while(fscanf (eFile, "vn %f %f %f\n", &f1, &f2, &f3)){
		vertexNormals.push_back(glm::vec3(f1,-f3,f2));
		//std::cout << "vn1:" << f1 << "  vn2:" << f2 << "  vn3:" << f3 << std::endl;
	}

	for(int i = 0; i < 2; i++){
		fscanf (eFile, "%s\n", &remove);
		//std::cout << remove << std::endl;
	}

	int i = 0;
  	while(fscanf (eFile, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9) != EOF){

  		planet.verts.push_back(glm::vec4(vertices[i1-1], 1.0f));
  		planet.verts.push_back(glm::vec4(vertices[i4-1], 1.0f));
  		planet.verts.push_back(glm::vec4(vertices[i7-1], 1.0f));

  		planet.uvs.push_back(vertexTextures[i2-1]);
  		planet.uvs.push_back(vertexTextures[i5-1]);
  		planet.uvs.push_back(vertexTextures[i8-1]);

  		planet.normals.push_back(glm::vec4(vertexNormals[i3-1], 1.0f));
  		planet.normals.push_back(glm::vec4(vertexNormals[i6-1], 1.0f));
  		planet.normals.push_back(glm::vec4(vertexNormals[i9-1], 1.0f));

  		/*
  		std::cout << "planet.normal1x: " << planet.normals[i][0] << "  planet.normal1y: " << planet.normals[i][1] << "  planet.normal1z: " << planet.normals[i][2] << std::endl;
  		i++;
  		std::cout << "planet.normal2x: " << planet.normals[i][0] << "  planet.normal2y: " << planet.normals[i][1] << "  planet.normal2z: " << planet.normals[i][2] << std::endl;
  		i++;
  		std::cout << "planet.normal3x: " << planet.normals[i][0] << "  planet.normal3y: " << planet.normals[i][1] << "  planet.normal3z: " << planet.normals[i][2] << std::endl;
  		i++;*/
  	}


	fclose (eFile);
	printf ("\nPlanet IO finished\n");

	return planet;
}


void Scene::addPlanet(Geometry &planet){
	planet.drawMode = GL_TRIANGLES;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(planet);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(planet);

	//Add the triangle to the scene objects
	objects.push_back(planet);
}



void Scene::generateSceneGraph(){
	sun.name = "sun";
	earth.name = "earth";
	eMoon.name = "eMoon";
	mars.name = "mars";

	earth.setParent(&sun);
	eMoon.setParent(&earth);
	mars.setParent(&sun);
}






















