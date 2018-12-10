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
uniform int specialFlag = 0;

uniform sampler2D imageTexture;
uniform sampler2D cloudTexture;
uniform sampler2D darkEarthTexture;
uniform sampler2D specularMapTexture;

void main(void)
{
    // write colour output without modification
    //FragmentColour = vec4(Colour, 0);

    
	FragmentColour = texture(imageTexture, uv);
	
	
	if(2.5 <= length(position) && length(position) <= 350){
		vec4 lightVector = (lightSource - position);
		vec4 normalN = normalize(normal);
		vec4 lightVectorN = normalize(lightVector);
		
		float lightEnhance = 0.7;
		float ambientLight = 0.5;
		vec4 R = -lightVectorN+((2*dot(lightVectorN,normalN))*normalN);
		vec4 V = normalize(eye-vec4(position.xyz, 0.0f));
		
		float NL = dot(normalN,lightVectorN);
		float RV = dot(R,V);
		if(RV < 0.0 || NL < -0.3){
			RV = 0;
		}

		float phi = 8;
		float RVP = pow(RV,phi);
		
		vec4 white = vec4(1,1,1,0);
		vec4 colour;
		
		
		if(specialFlag == 1){
			float waterSpecular = (texture(specularMapTexture,uv).x);
			colour = (lightEnhance*((NL*FragmentColour)+(0.3*RVP*(4*waterSpecular)*white)));
			}
			
		else
			colour = (lightEnhance*((NL*FragmentColour)+(0.6*RVP*white)));
			
			
		FragmentColour = vec4(colour.xyz, 0.0f) +(ambientLight*FragmentColour);
		
		if(specialFlag == 1){
			if(NL < 0)
				FragmentColour = mix(FragmentColour, texture(darkEarthTexture,uv)*1.3, -NL+0.1); 
			
			float alphaC = (texture(cloudTexture,uv).x) -0.1;
			if(alphaC < 0)
				alphaC = 0;
			FragmentColour = mix(FragmentColour, texture(cloudTexture,uv), alphaC);
		}
	}
}



