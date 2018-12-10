// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

vec4 lightSource = vec4(0,0,0,0);

// interpolated colour received from vertex stage
in vec4 position;
in vec2 uv;
in vec4 normal;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform vec4 eye;

uniform sampler2D imageTexture;

void main(void)
{
    // write colour output without modification
    //FragmentColour = vec4(Colour, 0);

    
	FragmentColour = texture(imageTexture, uv);
	
	///*
	if(2.5 <= length(position) && length(position) <= 350){
		vec4 lightVector = (lightSource - position);
		vec4 normalN = normalize(normal);
		vec4 lightVectorN = normalize(lightVector);
		
		float lightEnhance = 1.0;
		float ambientLight = 0.7;
		vec4 R = -lightVectorN+((2*dot(lightVectorN,normalN))*normalN);
		vec4 V = normalize(eye-vec4(position.xyz, 0.0f));
		
		float NL = dot(normalN,lightVectorN);
		float RV = dot(R,V);
		if(RV < 0.0 || NL < -0.1){
			RV = 0;
		}

		float phi = 4;
		float RVP = pow(RV,phi);
		
		vec4 white = vec4(1,1,1,0);
		vec4 colour = (lightEnhance*((NL*FragmentColour)+(0.6*RVP*white)));
		
		FragmentColour = vec4(colour.xyz, 0.0f) +(ambientLight*FragmentColour);
	}//*/
	
	
}
















/*
    vec4 red = vec4(1,0,0,0);
    vec4 green = vec4(0,1,0,0);
    vec4 blue = vec4(0,0,1,0);
    
    vec4 purple = vec4(1,0,1,0);
    
	if(angle>=0.5){	
		FragmentColour = purple;
	}
	else if(angle>=0){	
		FragmentColour = green;
	}
	else if (angle<0){
		FragmentColour = red;
	}	
	else{
		FragmentColour = blue;
	}*/
