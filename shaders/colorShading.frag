#version 120

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform float multiplier;
uniform float rowMultiplier;
uniform float colMultiplier;

uniform sampler2D mySampler;
uniform sampler2D mySampler2;
uniform sampler2D heightMap;

uniform vec3 lightPos;

void main()
{	
	vec4 textureColor = texture2D(mySampler, fragmentUV);
	vec4 textureColor2 = texture2D(mySampler2, fragmentUV);
	
	//get neighbouring points
	//!!!!!!watch for left and upper margin
	vec3 leftLowerPoint  = vec3(fragmentPosition.x - 0.5, 0, fragmentPosition.z + 0.5);
	vec3 rightLowerPoint = vec3(fragmentPosition.x + 0.5, 0, fragmentPosition.z + 0.5);
	vec3 rightUpperPoint = vec3(fragmentPosition.x + 0.5, 0, fragmentPosition.z - 0.5);
	
	//get colour from texture for each neighbour
	vec4 llpColor = texture2D(heightMap, vec2(leftLowerPoint.x*colMultiplier, leftLowerPoint.z*rowMultiplier));
	vec4 rlpColor = texture2D(heightMap, vec2(rightLowerPoint.x*colMultiplier, rightLowerPoint.z*rowMultiplier));
	vec4 rupColor = texture2D(heightMap, vec2(rightUpperPoint.x*colMultiplier, rightUpperPoint.z*rowMultiplier));
	
	//get back height from colour
	leftLowerPoint.y  = ((llpColor.r * 31 * 64 + llpColor.g * 63) * 32 + llpColor.b * 31)/multiplier;
	rightLowerPoint.y = ((rlpColor.r * 31 * 64 + rlpColor.g * 63) * 32 + rlpColor.b * 31)/multiplier;
	rightUpperPoint.y = ((rupColor.r * 31 * 64 + rupColor.g * 63) * 32 + rupColor.b * 31)/multiplier;
	
	//compute normal for triangle based on the 3 neighbouring points
	vec3 A = leftLowerPoint - rightLowerPoint;
	vec3 B = rightLowerPoint - rightUpperPoint;
	vec3 fragmentNormal2 = normalize(cross(A,B));

	//light computation
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//float dist = length(lightPos - fragmentPosition);
	vec3 lightVector = normalize(lightPos - fragmentPosition);
	float diffuse = max(0.3, dot(fragmentNormal2, lightVector));
	//attenuate light
	//diffuse = diffuse * (1.0 / (1.0 + 0.00001*dist*dist));
	gl_FragColor = fragmentColor * textureColor * vec4(lightColor*diffuse, 1.0);
}