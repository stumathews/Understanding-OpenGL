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

// Internal data structures
struct LightInfo
{
	vec4 position; // in eye-co-odinates 
	vec3 La;	   // Light's ambiant reflectance property (uniform brightness across the surface)
	vec3 Ld;	   // Light's diffuse reflectance property (brightness changes based on position/angle of incoming light angle)
	vec3 Ls;	   // Light's specular reflection poperty (brightness changes based on viewing position/angle)
	
	// Spot light characteristics
	vec3 direction;		// Which direction the light is pointing
	float exponent;		// Spot Factor
	float cutoff;		// The maximum angle that defines how wide the spotlight cone will be
};

struct MaterialInfo
{
	vec3 Ma;			// Materials ambiant reflactance property
	vec3 Md;			// Material's diffuse reflectance property
	vec3 Ms;			// Material's specular reflection property 
	float shininess;   // The higher the shininess the narrower or focused the highlight is on the surface
};

uniform LightInfo spotlight1; 
uniform LightInfo spotlight2; 
uniform MaterialInfo material1; 

// More computationally efficient
vec3 BlinnPhongSpotlightModel(LightInfo light, vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light.position - p));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.Ma;
	if (angle < cutoff) {
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(-p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = light.Ld * material1.Md * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
		specular = light.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
		return ambient + spotFactor * (diffuse + specular);
	} 
	else
		return ambient;
}

void main()
{
	// Get the colour of the point on the surface. Note we will get the ambiant colour if the 
	// position of the vertex on the surface is not within the spotlight - so it will
	// be the default non-spotlight light intensity colour (but the default or shade intensity of the colour)

	// Get the surface colour at point using its normal n and lighting characteristics from light 1
	vec3 light1Output = BlinnPhongSpotlightModel(spotlight1, point, normalize(n));

	// Get the surface colour at point using its normal n and lighting characteristics from light 2
	vec3 light2Output = BlinnPhongSpotlightModel(spotlight2, point, normalize(n));
	
	// Our final colour produced in this shader
	vec3 vColour;
		
	vColour = light1Output + light2Output;

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
