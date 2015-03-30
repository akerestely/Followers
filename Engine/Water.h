#pragma once
#include "GLSLProgram.h"
#include "CameraSpectator.h"

namespace Engine
{
	class Water
	{
	public:
		Water(void);
		~Water(void);

		void Render(const CameraSpectator &camera, float deltaTime);
	private:
		void buildModel();
		void initShader();

		unsigned int vboId, iboId;

		GLSLProgram *program;
	};
}

