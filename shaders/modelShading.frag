#version 120

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec2 fragmentUV;

uniform sampler2D mySampler;

uniform vec3 lightPos;
uniform vec3 lightColor;

const float ambientLight = 0.05;
const float minLight = -0.2;
const float maxLight = 0.1;

void main()
{	
	vec4 textureColor = texture2D(mySampler, fragmentUV);
	//discards fragment in case texture has alpha less than 0.9 (example: PNG files)
	if(textureColor.a < 0.9)
		discard;
	//frag shader 4
	//float dist = length(lightPos - fragmentPosition);
	vec3 lightVector = normalize(lightPos);
	float attenuation = (clamp(lightVector.y, minLight, maxLight)-minLight) / (maxLight - minLight);

	float diffuse = max(ambientLight, dot(fragmentNormal, lightVector)*attenuation);
	
	gl_FragColor = textureColor * vec4(lightColor*diffuse, 1.0);
}