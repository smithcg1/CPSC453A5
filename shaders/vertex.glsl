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

// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;

void main()
{
    // assign vertex position without modification
    vec4 transformedVertex = transform*VertexPosition;
   	//gl_Position = VertexPosition;
	gl_Position = transformedVertex;

    // assign output colour to be interpolated
    uv = VertexUV;
}
