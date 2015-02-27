#version 120

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform sampler2D mySampler;

uniform vec3 lightPos;

void main()
{							
	vec4 textureColor = texture2D(mySampler, fragmentUV);
	
	//frag shader 4
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//float dist = length(lightPos - fragmentPosition);
	vec3 lightVector = normalize(lightPos - fragmentPosition);
	float diffuse = max(0.1, dot(fragmentNormal, lightVector));
	//atenuate light
	//diffuse = diffuse * (1.0 / (1.0 + 0.00001*dist*dist));
	gl_FragColor = textureColor * vec4(lightColor*diffuse, 1.0);
}