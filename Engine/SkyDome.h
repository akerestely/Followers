#pragma once

#include "CameraSpectator.h"
#include "GLSLProgram.h"

namespace Engine
{
	class SkyDome
	{
	public:
		// Builds a Sky Dome, and uploads it to GPU
		SkyDome();
		~SkyDome();

		void Render(const CameraSpectator &camera, const glm::vec3 sunPos);

	private:
		void initShader();

		unsigned int vboId, iboId;
		GLSLProgram *skyProgram;

		//position of viewer in terms of sky dome
		const float InnerRadius;
	};
}


