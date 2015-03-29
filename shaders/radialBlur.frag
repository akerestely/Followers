#version 120

varying vec2 fragmentUV;

uniform vec2 sourcePos;
uniform sampler2D image;

void main()
{
	vec3 radialBlur = vec3(0.0);
	vec2 texCoord = fragmentUV;
	int samples = 128;
	vec2 radialBlurVector = (sourcePos - texCoord) / samples;

	for(int i = 0; i < samples; i++)
	{
		radialBlur += texture2D(image, texCoord).rgb;
		texCoord += radialBlurVector;
	}

	gl_FragColor = vec4(radialBlur /= samples, 1.0);
}