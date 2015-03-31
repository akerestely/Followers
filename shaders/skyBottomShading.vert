#version 120

attribute vec3 vertexPosition;

varying vec3 fragmentPosition;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	fragmentPosition = vec3(gl_Position);
}