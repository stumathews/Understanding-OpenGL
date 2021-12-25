#version 330 core

// vertex shader provided Parameters
in vec3 inColor; // vertex colour passed in by the vertex shader
in vec2 TextCoord;  // vertex's texture co-ordinates passed in by the vertex shader
in vec4 point;	// Vertex position in camera/eye-coorindates
in vec3 n;      // Vertex's normal in camera/eye-coodinates

// Client provided paramters
uniform sampler2D ourTexture1;
uniform bool useTexture;
uniform bool useCustomMaterial; // Use the material properties
uniform bool mixWithVertexColour; // Mix in the vertex' colour into the resulting colour (vertex can have its own colour along with texture's colour)

// Results
out vec4 color;	// We make colours in the fragment shader!




void main()
{

	vec3 vColour;
	vColour = inColor;

	if(useTexture)
	{
		if(mixWithVertexColour)
			color = texture(ourTexture1, TextCoord) * vec4(inColor, 1.0);
		else		
			color = texture(ourTexture1, TextCoord);
	}
	else
	{
		color = vec4(vColour, 1.0);	
	}
}
