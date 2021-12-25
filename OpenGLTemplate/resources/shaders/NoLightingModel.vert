#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 textcoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 inColor;
out vec2 TextCoord;


void main()
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);	
	inColor = color;
	TextCoord = textcoord;
}
