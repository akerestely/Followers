#version 120

varying vec2 fragmentUV;

uniform sampler2D texture, textureDepth, sceneDepth;

void main()
{
	if(texture2D(sceneDepth, fragmentUV).r < texture2D(textureDepth, fragmentUV).r)
	{
		gl_FragColor = vec4(vec3(0.0), 1.0);
	}
	else
	{
		gl_FragColor = vec4(texture2D(texture, fragmentUV).rgb, 1.0);
	}
}
