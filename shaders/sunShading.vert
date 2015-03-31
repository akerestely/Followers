#version 120

attribute vec3 vertexPosition;

uniform vec4 clipPlane;
uniform mat4 MVP;
uniform mat4 M;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	gl_ClipDistance[0] = dot(clipPlane, M * vec4(vertexPosition, 1.0));
}