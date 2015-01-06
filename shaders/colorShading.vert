#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;
attribute vec2 vertexUV;

varying vec3 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform mat4 P;

struct lightSource
{
  vec4 position;
  vec4 diffuse;
};
lightSource light0 = lightSource(
    vec4(-1.0, 1.0, -1.0, 0.0),
    vec4(1.0, 1.0, 1.0, 1.0)
);
 
struct material
{
  vec4 diffuse;
};
material mymaterial = material(vec4(1.0, 1.0, 1.0, 1.0));

void main()
{
	vec3 normalDirection = normalize(/*m_3x3_inv_transp **/ vertexNormal);
	vec3 lightDirection = normalize(vec3(light0.position));
 
	vec3 diffuseReflection = vec3(light0.diffuse) * vec3(mymaterial.diffuse) * max(0.0, dot(normalDirection, lightDirection));
 
	gl_Position = P * vec4(vertexPosition, 1.0);
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor*vec4(diffuseReflection, 1.0);
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}