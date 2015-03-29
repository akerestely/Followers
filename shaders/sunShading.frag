#version 120

uniform vec3 sunColor;

void main()
{
	gl_FragColor = vec4(sunColor, 1.0);						
}