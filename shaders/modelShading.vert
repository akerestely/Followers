#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexUV;

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec2 fragmentUV;

uniform mat4 M;
uniform mat4 MVP;

void main()
{	 	
	fragmentPosition = (M * vec4(vertexPosition, 1.0)).xyz;
	fragmentNormal = (vec4(vertexNormal, 1.0)).xyz;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
	
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}