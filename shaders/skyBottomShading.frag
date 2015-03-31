#version 120

varying vec3 fragmentPosition;

uniform vec3 sunDir;

const float minLight = -0.2;
const float maxLight = 0.1;

void main()
{
	vec3 fogColor = vec3(0.5, 0.5, 0.5);
	float attenuation = (clamp(sunDir.y, minLight, maxLight)-minLight) / (maxLight - minLight);
	vec3 skyColor = vec3(0.0, 0.5, 1.0) * attenuation;
	gl_FragColor = vec4(skyColor*fogColor*fragmentPosition.z, 1.0);						
}