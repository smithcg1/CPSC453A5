/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "Geometry.h"
#include <string>

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

private:
	RenderingEngine* renderer;

	Geometry space;
	Geometry sun;
	Geometry mercury;
	Geometry venus;
	Geometry earth;
	Geometry eMoon;
	Geometry mars;
	Geometry jupiter;
	Geometry saturn;
	Geometry uranus;
	Geometry neptune;

	void createPlanet(Geometry &planet, std::string fileName, std::string textureName, float axialTilt, float inclination, float planetR, float rotationPeriod, long int orbitR, float orbitPeriod);

	Geometry readPlanet(Geometry &planet, std::string fileName);
	void addPlanet(Geometry &planet);

	//list of objects in the scene
	std::vector<Geometry> objects;

	void generateSceneGraph();
};

#endif /* SCENE_H_ */
