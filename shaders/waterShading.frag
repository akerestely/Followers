#version 120

varying vec2 fragmentUV;

uniform sampler2D waterTexture;
 
void main() {
    gl_FragColor = texture2D(waterTexture, fragmentUV);
}