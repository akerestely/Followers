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
		void buildBottom();

		unsigned int vboId, iboId, vboIdBottom;
		GLSLProgram *skyProgram, *skyBottomProgram;

		//position of viewer in terms of sky dome
		const float InnerRadius;
	};
}


