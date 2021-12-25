#version 410 core

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

uniform bool isRenderingInstances;

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inColour;
layout (location = 4) in mat4 instanceMatrix;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
//out vec3 vColour;	// Colour computed using reflectance model
out vec2 vTexCoord;	// Texture coordinate
out vec3 worldPosition;	// used for skybox
out vec3 colour;

out vec4 point;
out vec3 n;

// This is the entry point into the vertex shader
void main()
{
    // Save the world position for rendering the skybox
	worldPosition = inPosition;

	vec4 vEyePosition;
	
	// Transform the vertex spatial position using 
	if(isRenderingInstances)
	{
		gl_Position = matrices.projMatrix * instanceMatrix * vec4(inPosition, 1.0f);
		vEyePosition = instanceMatrix * vec4(inPosition, 1.0f);
	}
	else
	{
		gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);
		vEyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	}
	
	// Get the vertex normal and vertex position in eye coordinates
	vec3 vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	
	
	// Pass through the texture coordinate
	vTexCoord = inCoord;
	colour = inColour;

	n = vEyeNorm;
	point = vEyePosition;

	
} 
	