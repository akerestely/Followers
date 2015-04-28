#pragma once

#include "Model.h"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>

namespace Engine
{
	struct BoneInfo
	{
		glm::mat4 boneOffset;
		glm::mat4 finalTransformation;        

		BoneInfo()
		{
			boneOffset = finalTransformation = glm::mat4(0.0);            
		}
	};

	class AnimModel : public Model
	{
		friend class ModelLoader;

	public:
		AnimModel(void);
		~AnimModel(void);

		void Update(float time) override;
		void Render(const CameraSpectator &camera, const Sun *sun) override;

	private:
		void initShader() override;
		void boneTransform(float frames);
		void readNodeHeirarchy(float animationTime, const aiNode* node, const glm::mat4 &parentTransform);
		const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string nodeName);
		void calcInterpolatedPosition(aiVector3D& Out, float animationTime, const aiNodeAnim* nodeAnim);
		void calcInterpolatedRotation(aiQuaternion& Out, float animationTime, const aiNodeAnim* nodeAnim);
		void calcInterpolatedScaling(aiVector3D& Out, float animationTime, const aiNodeAnim* nodeAnim);
		unsigned int findPosition(float animationTime, const aiNodeAnim* nodeAnim);
		unsigned int findRotation(float animationTime, const aiNodeAnim* nodeAnim);
		unsigned int findScaling(float animationTime, const aiNodeAnim* nodeAnim);

		glm::mat4 copyAiMat(const aiMatrix4x4 &from);

		std::map<std::string, unsigned int> boneMapping;
		std::vector<BoneInfo> boneInfo;
		Assimp::Importer importer;
		const aiScene *scene;
	};
}


