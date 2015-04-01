#pragma once

#include "Mesh.h"

#include <vector>
#include "GLSLProgram.h"
#include "CameraSpectator.h"
#include "Sun.h"

namespace Engine
{
	class Model
	{
		friend class ModelLoader;

	public:
		Model(void);
		~Model(void);

		void Render(const CameraSpectator &camera, const Sun *sun);

		//public variables
		float RotateY;
		glm::vec3 Position;
	private:
		void initShader();

		std::vector<Mesh> meshes;
		std::vector<int> materials;
		GLSLProgram *program;
	};
}


