#version 120

attribute vec2 vertexPosition;
attribute vec4 vertexColor;

varying vec2 fragmentPosition;
varying vec4 fragmentColor;

void main()
{
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
}