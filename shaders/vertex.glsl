// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec4 VertexNormal;

uniform mat4 transform;		//Contains Persp*View*Model
uniform mat4 planetRotation;	//Just planet rotation
uniform mat4 planetTransform;	//Model matrix

// output to be interpolated between vertices and passed to the fragment stage
out vec4 position;
out vec2 uv;
out vec4 normal;

void main()
{
    // assign vertex position without modification
    vec4 transformedVertex = transform*VertexPosition;
   	//gl_Position = VertexPosition;
	gl_Position = transformedVertex;

    // assign output colour to be interpolated
    position = planetTransform*VertexPosition;
    uv = VertexUV;
    normal = planetRotation*VertexNormal;
}
