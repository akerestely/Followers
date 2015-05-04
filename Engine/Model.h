#pragma once

#include <vector>
#include <map>
#include <string>

#include "GLSLProgram.h"
#include "CameraSpectator.h"
#include "Sun.h"

namespace Engine
{
	struct Mesh
	{
		unsigned int nIndices;
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int materialIndex;
	};

	class Model
	{
	public:
		friend class ModelLoader;
		Model(void);
		virtual ~Model(void);

		virtual void Update(float time);
		virtual void Render(const CameraSpectator &camera, const Sun *sun);

		//public variables
		float RotateY;
		glm::vec3 Position;
		float Scale;
	protected:
		virtual void initShader();
		glm::mat4 computeModelMatrix();

		std::vector<Mesh> meshes;
		std::vector<int> materials;
		GLSLProgram *program;
		unsigned int vboId, iboId;
	};
}

