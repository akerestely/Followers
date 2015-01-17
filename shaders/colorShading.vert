#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;
attribute vec2 vertexUV;

varying vec3 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;

uniform vec3 lightPos;

void main()
{	 
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float dist = length(lightPos - vertexPosition);
	vec3 lightVector = normalize(lightPos - vertexPosition);
	float diffuse = max(0.1, dot(lightVector, vertexNormal));
	//atenuate light
	diffuse = diffuse * (1.0 / (1.0 + 0.00001*dist*dist));
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor * vec4(lightColor * diffuse, 1.0);
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}