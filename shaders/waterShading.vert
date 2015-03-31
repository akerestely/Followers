#version 120

attribute vec3 vertexPosition;

varying vec3 fragmentPosition;
varying vec2 fragmentUV;

uniform float time;
uniform float waveWidth;
uniform float waveHeight;
uniform mat4 MVP;

void main()
{
	fragmentUV = vertexPosition.xz/1000;
	
	vec3 v = vertexPosition;
    v.y += sin(waveWidth * v.x + time) * cos(waveWidth * v.z + time) * waveHeight;
    gl_Position = MVP * vec4(v, 1.0);
	fragmentPosition = vec3(gl_Position);
}