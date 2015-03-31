#version 120

varying vec3 fragmentPosition;
varying vec2 fragmentUV;

uniform sampler2D waterTexture;
uniform float fogDistance;
 
void main() {
    gl_FragColor = texture2D(waterTexture, fragmentUV) * 1 - fragmentPosition.z/fogDistance;
}