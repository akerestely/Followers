#version 120

varying vec2 fragmentPosition;
varying vec4 fragmentColor;
varying vec2 fragmentUV;

uniform sampler2D mySampler;

void main()
{							
	vec4 textureColor = texture2D(mySampler, fragmentUV);
	gl_FragColor = fragmentColor * textureColor;
}