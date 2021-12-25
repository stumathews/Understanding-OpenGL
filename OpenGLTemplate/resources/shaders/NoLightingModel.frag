#version 330 core

// vertex shader provided Parameters
in vec3 inColor; // vertex colour passed in by the vertex shader
in vec2 TextCoord;  // vertex's texture co-ordinates passed in by the vertex shader

// Client provided paramters
uniform sampler2D ourTexture1;
uniform bool useTexture;
uniform bool useCustomMaterial; // Use the material properties
uniform bool mixWithVertexColour; // Mix in the vertex' colour into the resulting colour (vertex can have its own colour along with texture's colour)

// Results
out vec4 color;	// We make colours in the fragment shader!

void main()
{
	color = vec4(inColor, 1.0);		
}
