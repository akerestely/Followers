#version 120

attribute vec3 vertexPosition;

varying vec2 fragmentUV;

void main()
{
	fragmentUV = (vertexPosition.xy + vec2( 1.0 ) ) / vec2( 2.0 );
	gl_Position = vec4(vertexPosition, 1.0);
}