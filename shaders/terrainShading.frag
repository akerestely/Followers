#version 120

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec2 fragmentUV;

uniform sampler2D mySampler0;
uniform sampler2D mySampler1;
uniform sampler2D mySampler2;

uniform float maxHeight;
uniform float minHeight;

uniform vec3 lightPos;
uniform vec3 lightColor;

float ambientLight = 0.05;
const float minLight = -0.2;
const float maxLight = 0.1;

void main()
{
	float interval = maxHeight - minHeight;
	float fScale = (fragmentPosition.y - minHeight) / interval;
	
	const float fRange1 = 0.2f;
	const float fRange2 = 0.3f;
	const float fRange3 = 0.6f;
	const float fRange4 = 0.7f;
	
	vec4 vTexColor = vec4(0.0);
	if(fScale >= 0.0 && fScale <= fRange1)vTexColor = texture2D(mySampler0, fragmentUV);
	else if(fScale <= fRange2)
	{
		fScale -= fRange1;
		fScale /= (fRange2-fRange1);
	  
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
	  
		vTexColor += texture2D(mySampler0, fragmentUV)*fScale;
		vTexColor += texture2D(mySampler1, fragmentUV)*fScale2;
	}
	else if(fScale <= fRange3)vTexColor = texture2D(mySampler1, fragmentUV);
	else if(fScale <= fRange4)
	{
		fScale -= fRange3;
		fScale /= (fRange4-fRange3);
	  
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
	  
		vTexColor += texture2D(mySampler1, fragmentUV)*fScale;
		vTexColor += texture2D(mySampler2, fragmentUV)*fScale2;      
	}
	else vTexColor = texture2D(mySampler2, fragmentUV); 

	//float dist = length(lightPos - fragmentPosition);
	vec3 lightVector = normalize(lightPos);
	
	float attenuation = (clamp(lightVector.y, minLight, maxLight)-minLight) / (maxLight - minLight);
	
	float diffuse = max(ambientLight, dot(fragmentNormal, lightVector)*attenuation);
	
	gl_FragColor = vTexColor * vec4(lightColor*diffuse, 1.0);
}