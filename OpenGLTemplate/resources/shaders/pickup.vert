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
out vec4 point;
out vec3 n;


void main()
{
	// transform the vertex position to projection space
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

	// Get the vertex normal in eye/camera coordinates
	vec3 vn = normalize(normalMatrix * normal);

	// Get the vertex position in eye/camera coordinates
	vec4 vp = modelViewMatrix * vec4(position, 1.0f);

	// Send out the 1) the colour of the vertex, 2) vertex texture co-coodinate, 3) vertex normal and 4) vertex position
	inColor = color;  // 1
	TextCoord = textcoord; // 2
	n = vn; // 3
	point = vp; // 4


}
