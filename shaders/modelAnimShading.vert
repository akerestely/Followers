#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexUV;
attribute vec4 boneIDs;
attribute vec4 weights;

varying vec3 fragmentPosition;
varying vec3 fragmentNormal;
varying vec2 fragmentUV;

const int MAX_BONES = 100;

uniform mat4 M;
uniform mat4 MVP;

uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 boneTransform = gBones[int(boneIDs[0])] * weights[0];
    boneTransform += gBones[int(boneIDs[1])] * weights[1];
    boneTransform += gBones[int(boneIDs[2])] * weights[2];
    boneTransform += gBones[int(boneIDs[3])] * weights[3];

	vec4 pos = boneTransform * vec4(vertexPosition, 1.0);
	
	fragmentPosition = (M * pos).xyz;
	fragmentNormal = (boneTransform * vec4(vertexNormal, 0.0)).xyz;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);

	gl_Position = MVP * pos;
}