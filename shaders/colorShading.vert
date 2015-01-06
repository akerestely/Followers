#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;
attribute vec2 vertexUV;

varying vec3 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform mat4 P;
uniform vec3 lightPos;
uniform mat3 inverseMatrix;

struct lightSource
{
  vec4 position;
  vec4 diffuse;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
};
lightSource light0 = lightSource(vec4(lightPos,1.0), vec4(1.0, 1.0, 1.0, 1.0), 1.0, 0.0, 0.0 );
 
struct material
{
  vec4 diffuse;
};
material mymaterial = material(vec4(1.0, 1.0, 1.0, 1.0));

void main()
{	
	vec3 normalDirection = normalize(/*inverseMatrix * */vertexNormal);
	vec3 lightDirection;
	float attenuation;
	 
	if (light0.position.w == 0.0) // directional light
	{
		attenuation = 1.0; // no attenuation
		lightDirection = normalize(vec3(light0.position));
	}
	else // point or spot light
	{ 
		vec3 vertexToLightSource = vec3(light0.position - /*m * */ vertexPosition);
		float distance = length(vertexToLightSource);
		attenuation = 1.0 / (light0.constantAttenuation 
						   + light0.linearAttenuation * distance
						   + light0.quadraticAttenuation * distance * distance);
		lightDirection = normalize(vertexToLightSource);
	}
 
	vec3 diffuseReflection = attenuation * vec3(light0.diffuse) * vec3(mymaterial.diffuse) * max(0.0, dot(normalDirection, lightDirection));
 
	gl_Position = P * vec4(vertexPosition, 1.0);
	
	fragmentPosition = vertexPosition;
	fragmentColor = vec4(diffuseReflection, 1.0);
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}