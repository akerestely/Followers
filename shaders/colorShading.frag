#version 120

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform float multiplier;
uniform float rowSize;
uniform float colSize;
uniform float nRows;
uniform float nCols;

uniform sampler2D mySampler;
uniform sampler2D mySampler2;
uniform sampler2D heightMap;

uniform vec3 lightPos;

float getHeight(vec2 point2d)
{
	//get grid(texture) boundaries
	int x1 = int(point2d.x/colSize);
	int x2 = x1+1;
	int y1 = int(point2d.y/rowSize);
	int y2 = y1+1;
	
	float f00 = texture2D(heightMap, vec2(x1/nCols, y1/nRows)).r*65535/multiplier;
	float f10 = texture2D(heightMap, vec2(x2/nCols, y1/nRows)).r*65535/multiplier;
	float f11 = texture2D(heightMap, vec2(x2/nCols, y2/nRows)).r*65535/multiplier;
	float f01 = texture2D(heightMap, vec2(x1/nCols, y2/nRows)).r*65535/multiplier;
	
	float px = point2d.x/colSize - x1;
	float py = point2d.y/rowSize - y1;
	
	float fFinal = f00 * (1.0 - px) * (1.0 - py) +
		f10 * px * (1.0 - py) + f01 * (1.0 - px) * py + f11 * px * py;
	return fFinal;
}

void main()
{	
	vec4 textureColor = texture2D(mySampler, fragmentUV);
	vec4 textureColor2 = texture2D(mySampler2, fragmentUV);
	
	//get neighbouring points
	//!!!!!optimize using  MAD!!!
	//!!!!!!watch for left and upper margin
	vec3 leftLowerPoint  = vec3(fragmentPosition.x - 0.5, 0, fragmentPosition.z + 0.5);
	vec3 rightLowerPoint = vec3(fragmentPosition.x + 0.5, 0, fragmentPosition.z + 0.5);
	vec3 rightUpperPoint = vec3(fragmentPosition.x + 0.5, 0, fragmentPosition.z - 0.5);
	
	//get back height from colour
	leftLowerPoint.y  = getHeight(leftLowerPoint.xz);
	rightLowerPoint.y = getHeight(rightLowerPoint.xz);
	rightUpperPoint.y = getHeight(rightUpperPoint.xz);
	
	// if(rightUpperPoint.y == rightLowerPoint.y)
	//	discard;
	// if(leftLowerPoint.y == rightLowerPoint.y && rightLowerPoint.y == rightUpperPoint.y)
	// 	discard;
	
	// float h = getHeight(vec2(100,153));
	// float e = 5;
	// float rez = 1871.9640;
	// if(rez-e < h && h > rez+e)
		// discard;
		
	// if(getHeight(fragmentPosition.xz) > 1700)
		// discard;
	
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
	//diffuse = diffuse * (1.0 / (1.0 + 0.000001*dist*dist));
	gl_FragColor = fragmentColor * vec4(lightColor*diffuse, 1.0);
}