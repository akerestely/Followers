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

float getHeight2(vec2 point2d)
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

 float Triangular( float f )
{
	f = f / 2.0;
	if( f < 0.0 )
	{
		return ( f + 1.0 );
	}
	else
	{
		return ( 1.0 - f );
	}
	return 0.0;
}

float getHeight( vec2 tc )
{
	vec2 TexCoord = tc;
	float fWidth = nRows, fHeight = nCols;
	TexCoord.x = TexCoord.x / colSize / nCols; TexCoord.y = TexCoord.y / rowSize / nRows;
    float texelSizeX = 1.0 / fWidth; //size of one texel 
    float texelSizeY = 1.0 / fHeight; //size of one texel 
    float nSum = 0.0;
    float nDenom = 0.0;
    float a = fract( TexCoord.x * fWidth * multiplier); // get the decimal part
    float b = fract( TexCoord.y * fHeight * multiplier); // get the decimal part
    for( int m = -1; m <=2; m++ )
    {
        for( int n =-1; n<= 2; n++)
        {
			float vecData = texture2D(heightMap, 
                               TexCoord + vec2(texelSizeX * float( m ), 
					texelSizeY * float( n ))).r * 65535;
			float f  = Triangular( float( m ) - a );
			float vecCooef1 = f;
			float f1 = Triangular ( -( float( n ) - b ) );
			float vecCoeef2 = f1;
            nSum = nSum + ( vecData * vecCoeef2 * vecCooef1  );
            nDenom = nDenom + (( vecCoeef2 * vecCooef1 ));
        }
    }
    return nSum / nDenom;
}

void main()
{	
	vec4 textureColor = texture2D(mySampler, fragmentUV);
	vec4 textureColor2 = texture2D(mySampler2, fragmentUV);
	
	vec3 test = fragmentPosition;
	//test.x = 3.6;
	//test.z = 30 + 0.02;
	//test.y = 0;

	//get neighbouring points
	//!!!!!optimize using  MAD!!!
	//!!!!!!watch for left and upper margin
	vec3 leftLowerPoint  = vec3(test.x - 7.2, 0, test.z + 10);
	vec3 rightLowerPoint = vec3(test.x + 7.2, 0, test.z + 10);
	vec3 rightUpperPoint = vec3(test.x + 7.2, 0, test.z - 10);
	
	//get back height from colour
	leftLowerPoint.y  = getHeight(leftLowerPoint.xz);
	rightLowerPoint.y = getHeight(rightLowerPoint.xz);
	rightUpperPoint.y = getHeight(rightUpperPoint.xz);

	vec2 test2 = vec2( 7.2 * 3, 10 * 3);
	float height = getHeight( test2);
	if( height < 1800)
	discard;
		
	//compute normal for triangle based on the 3 neighbouring points
	vec3 A = leftLowerPoint - rightLowerPoint;
	vec3 B = rightLowerPoint - rightUpperPoint;
	vec3 fragmentNormal2 = normalize(cross(A,B));

	//light computation
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//float dist = length(lightPos - test);
	vec3 lightVector = normalize(lightPos - test);
	float diffuse = max(0.3, dot(fragmentNormal2, lightVector));

	//attenuate light
	//diffuse = diffuse * (1.0 / (1.0 + 0.000001*dist*dist));
	gl_FragColor = fragmentColor * vec4(lightColor*diffuse, 1.0);
}