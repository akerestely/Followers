#version 120

attribute vec2 vertexPosition;
attribute vec4 vertexColor;
attribute vec2 vertexUV;

varying vec2 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

void main()
{
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}