#version 120

varying vec2 fragmentPosition;
varying vec4 fragmentColor;

uniform float time;

void main()
{
	//frag shader 1
	// gl_FragColor[0] = gl_FragCoord.x/640.0;
	// gl_FragColor[1] = gl_FragCoord.y/480.0;
	// gl_FragColor[2] = 0.5;
	// if ( mod(gl_FragCoord.y, 30.0) > 15 )
		// gl_FragColor[3] = 1;
	// else
		// gl_FragColor[3] = 0.5;
	
	//frag shader 2
	// gl_FragColor = fragmentColor + vec4(1.0 * (cos(time) + 1.0) * 0.5,
										// 1.0 * (cos(time + 2.0) +1.0) * 0.5,
										// 1.0 * (sin(time + 1.0) +1.0) * 0.5, 0.0);
	
	//frag shader 3
	gl_FragColor = vec4(fragmentColor.r + (cos(fragmentPosition.x * 2.0 + time) + 1.0)* 0.5,
						fragmentColor.g + (cos(fragmentPosition.y * 4.0+ time) + 1.0)* 0.5,
						fragmentColor.b + (cos(fragmentPosition.x * 1.0 + time) + 1.0)* 0.5, fragmentColor.a);
}