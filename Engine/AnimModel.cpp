#include "AnimModel.h"
#include "Vertex.h"

#include <stddef.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Engine
{
	const  glm::vec3 OY(0.0f, 1.0f, 0.0f);
	AnimModel::AnimModel(void)
	{
		//empty
	}

	AnimModel::~AnimModel(void)
	{
		//empty
	}

	void AnimModel::Update(float time)
	{
		if(scene->mNumAnimations)
			boneTransform(time);
	}

	void AnimModel::Render(const CameraSpectator &camera, const Sun *sun)
	{
		if(!program)
			initShader();
		program->Use();
		glm::vec3 sunPos = glm::rotate(sun->GetSunPosition(), -RotateY, OY);
		glUniform3fv(program->GetUniformLocation("lightPos"), 1, &sunPos[0]);
		glUniform3fv(program->GetUniformLocation("lightColor"), 1, &sun->GetSunColor()[0]);
		glm::mat4 m = computeModelMatrix();
		glm::mat4 mvp = camera.GetCameraMatrix() * m;
		//glUniformMatrix4fv(program->GetUniformLocation("M"), 1, GL_FALSE, &m[0][0]);
		glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::BonedVertex), (void*)offsetof(Engine::BonedVertex,position));
		glVertexAttribPointer(1, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::BonedVertex), (void*)offsetof(Engine::BonedVertex,normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::BonedVertex), (void*)offsetof(Engine::BonedVertex,uv));
		glVertexAttribPointer(3, NUM_BONES_PER_VEREX, GL_UNSIGNED_INT, GL_FALSE, sizeof(Engine::BonedVertex), (void*)offsetof(Engine::BonedVertex,boneData.ids));
		glVertexAttribPointer(4, NUM_BONES_PER_VEREX, GL_FLOAT, GL_FALSE, sizeof(Engine::BonedVertex), (void*)offsetof(Engine::BonedVertex,boneData.weights));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

		glActiveTexture(GL_TEXTURE0);
		for (auto it = meshes.begin(); it != meshes.end(); ++it)
		{			
			glBindTexture(GL_TEXTURE_2D, materials[it->materialIndex]);
			glDrawElements(GL_TRIANGLES, it->nIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * it->baseIndex));
		}
		program->UnUse();
	}

	void AnimModel::initShader()
	{
		program = new GLSLProgram;

		program->CompileShaders("shaders/modelAnimShading.vert", "shaders/modelShading.frag");
		program->AddAttribute("vertexPosition");
		program->AddAttribute("vertexNormal");
		program->AddAttribute("vertexUV");
		program->AddAttribute("boneIDs");
		program->AddAttribute("weights");
		program->LinkShader();
	}

	void AnimModel::boneTransform(float frames)
	{	
		if(scene->mAnimations[0]->mTicksPerSecond == 1)
			frames /= 30;
		float animationTime = fmod(frames, (float)scene->mAnimations[0]->mDuration);

		readNodeHeirarchy(animationTime, scene->mRootNode, glm::mat4());
		
		glm::mat4 *boneTransformations = new glm::mat4[boneInfo.size()];
		for(unsigned int i = 0U; i < boneInfo.size(); i++)
			boneTransformations[i] = boneInfo[i].finalTransformation;

		if(!program)
			initShader();
		program->Use();
		unsigned int bonesLocation = program->GetUniformLocation("gBones[0]");
		glUniformMatrix4fv(bonesLocation, boneInfo.size(), GL_FALSE, &boneTransformations[0][0][0]);
		program->UnUse();

		delete boneTransformations;
	}

	void AnimModel::readNodeHeirarchy(float animationTime, const aiNode* node, const glm::mat4 &parentTransform)
	{
		std::string nodeName(node->mName.data);

		const aiAnimation* animation = scene->mAnimations[0];

		glm::mat4 nodeTransformation = copyAiMat(node->mTransformation);

		const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

		if (nodeAnim) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D scaling;
			calcInterpolatedScaling(scaling, animationTime, nodeAnim);
			glm::mat4 scalingM = glm::scale(glm::vec3(scaling.x, scaling.y, scaling.z));

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion rotationQ;
			calcInterpolatedRotation(rotationQ, animationTime, nodeAnim);
			glm::quat q(rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z);
			glm::mat4 rotationM = glm::mat4_cast(q);

			// Interpolate translation and generate translation transformation matrix
			aiVector3D translation;
			calcInterpolatedPosition(translation, animationTime, nodeAnim);
			glm::mat4 translationM = glm::translate(glm::vec3(translation.x, translation.y, translation.z));

			// Combine the above transformations
			nodeTransformation = translationM * rotationM * scalingM;
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransformation;

		if (boneMapping.find(nodeName) != boneMapping.end()) 
		{
			unsigned int boneIndex = boneMapping[nodeName];
			boneInfo[boneIndex].finalTransformation = globalTransformation * boneInfo[boneIndex].boneOffset;
		}

		for (unsigned int i = 0 ; i < node->mNumChildren ; i++)
			readNodeHeirarchy(animationTime, node->mChildren[i], globalTransformation);
	}

	const aiNodeAnim* AnimModel::findNodeAnim(const aiAnimation* animation, const std::string nodeName)
	{
		for (unsigned int i = 0 ; i < animation->mNumChannels ; i++) 
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName) 
				return nodeAnim;
		}
		return nullptr;
	}

	void AnimModel::calcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumPositionKeys == 1) {
			out = nodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int positionIndex = findPosition(animationTime, nodeAnim);
		unsigned int nextPositionIndex = (positionIndex + 1);
		float deltaTime = (float)(nodeAnim->mPositionKeys[nextPositionIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime);
		float factor = (animationTime - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
		const aiVector3D& start = nodeAnim->mPositionKeys[positionIndex].mValue;
		const aiVector3D& end = nodeAnim->mPositionKeys[nextPositionIndex].mValue;
		aiVector3D delta = end - start;
		out = start + factor * delta;
	}
	
	void AnimModel::calcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim)
	{
		// we need at least two values to interpolate...
		if (nodeAnim->mNumRotationKeys == 1) {
			out = nodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int rotationIndex = findRotation(animationTime, nodeAnim);
		unsigned int nextRotationIndex = (rotationIndex + 1);
		float deltaTime = (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
		float factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
		const aiQuaternion& start = nodeAnim->mRotationKeys[rotationIndex].mValue;
		const aiQuaternion& end   = nodeAnim->mRotationKeys[nextRotationIndex].mValue;    
		aiQuaternion::Interpolate(out, start, end, factor);
		out = out.Normalize();
	}
	
	void AnimModel::calcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumScalingKeys == 1) {
			out = nodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int scalingIndex = findScaling(animationTime, nodeAnim);
		unsigned int nextScalingIndex = (scalingIndex + 1);
		float deltaTime = (float)(nodeAnim->mScalingKeys[nextScalingIndex].mTime - nodeAnim->mScalingKeys[scalingIndex].mTime);
		float factor = (animationTime - (float)nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
		const aiVector3D& start = nodeAnim->mScalingKeys[scalingIndex].mValue;
		const aiVector3D& end   = nodeAnim->mScalingKeys[nextScalingIndex].mValue;
		aiVector3D delta = end - start;
		out = start + factor * delta;
	}

	unsigned int AnimModel::findPosition(float animationTime, const aiNodeAnim* nodeAnim)
	{    
		for (unsigned int i = 0 ; i < nodeAnim->mNumPositionKeys - 1 ; i++)
			if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
				return i;
		return 0;
	}
	
	unsigned int AnimModel::findRotation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (unsigned int i = 0 ; i < nodeAnim->mNumRotationKeys - 1 ; i++)
			if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime)
				return i;
	}
	
	unsigned int AnimModel::findScaling(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (unsigned int i = 0 ; i < nodeAnim->mNumScalingKeys - 1 ; i++) 
			if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime)
				return i;
		return 0;
	}

	glm::mat4 AnimModel::copyAiMat(const aiMatrix4x4 &from) 
	{
		glm::mat4 to;
		to[0][0] = from.a1; to[1][0] = from.a2;
		to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2;
		to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2;
		to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2;
		to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}
}

