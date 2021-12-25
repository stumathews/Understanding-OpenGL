#version 410 core

in vec2 vTexCoord;	// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec3 colour;
in vec3 worldPosition;
in vec4 point;	// Vertex position in camera/eye-coorindates
in vec3 n;      // Vertex's normal in camera/eye-coodinates 


out vec4 vOutputColour;		// The output colour



struct SpotLightInfo
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

struct LightInfo
{
	vec4 position; // in eye-co-odinates 
	vec3 La;	   // Light's ambiant reflectance property (uniform brightness across the surface)
	vec3 Ld;	   // Light's diffuse reflectance property (brightness changes based on position/angle of incoming light angle)
	vec3 Ls;	   // Light's specular reflection poperty (brightness changes based on viewing position/angle)
};

struct MaterialInfo
{
	vec3 Ma;			// Materials ambiant reflactance property
	vec3 Md;			// Material's diffuse reflectance property
	vec3 Ms;			// Material's specular reflection property 
	float shininess;   // The higher the shininess the narrower or focused the highlight is on the surface
};

uniform LightInfo sun;
uniform SpotLightInfo spotlight1; 
uniform SpotLightInfo spotlight2; 
uniform MaterialInfo material1; 
uniform bool useSpotlights;
uniform bool useConstantSunlight;
uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool bUseFog;
uniform bool renderSkybox;


// Fog properties
float rho = 0.01f;
float light_fog = 0.004;
vec3 fogColour = vec3(0.65f);

// Used for the sunlight - taken from course material
vec3 PhongModel(vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(sun.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 n = eyeNorm;
	vec3 ambient = sun.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = sun.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	
	if (sDotN > 0.0f) 
	{
		specular = sun.Ls * material1.Ms * pow(max(dot(r, v), 0.0f), material1.shininess + eps);
	}

	return ambient + diffuse + specular;

}

// modified to to use h to speed things up
vec3 BlinnPhongSpotlightModel(SpotLightInfo light, vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light.position - p));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.Ma;
	vec4 to_light = light.position - p;
	float d = length( to_light );
	float attenuation = clamp( 10.0 / d, 0.0, 1.0);

	if (angle < cutoff) 
	{
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(-p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = light.Ld * material1.Md * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
		specular = light.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
		return attenuation * (ambient + spotFactor * (diffuse + specular));
	} 
	else
	{
		return ambient;
	}
}

void main()
{


	if (renderSkybox)
	{
		vOutputColour = texture(CubeMapTex, worldPosition);
	} 
	else 
	{

		vec3 sunOutput;
		vec3 light1Output = vec3(0);
		vec3 light2Output = vec3(0);
		vec4 vTexColour = texture(sampler0, vTexCoord);	 // Get the texel colour from the texture sampler

		
		if(useConstantSunlight) 
		{
			sunOutput = vec3(1.0); // Sunlight is equally bright across ambient, diffuse, specular
		}
		else
		{
			sunOutput = PhongModel(point, normalize(n));
		}


		light1Output = BlinnPhongSpotlightModel(spotlight1, point, normalize(n));
		light2Output = BlinnPhongSpotlightModel(spotlight2, point, normalize(n));

		vec3 totalColour = sunOutput + light1Output + light2Output;


		if (bUseTexture)
		{
			vOutputColour = vTexColour  * vec4(totalColour, 1.0f);	// Combine object colour and texture 
		}
		else
		{
			vOutputColour = vec4(totalColour, 1.0f);	// Just use the colour instead
		}

		
		if(bUseFog)	// Make a fog effect
		{
			rho = light_fog;
			float d = length(point.xyz);
			//float w = exp(-rho*d/2);
			
			float raised2 = (rho * d) * (rho * d);
			float w = exp(-pow(rho*d,2));

			// Final output colour
			vOutputColour.rgb = mix(fogColour, vOutputColour.rgb, w );	
		}
	}
	
}
