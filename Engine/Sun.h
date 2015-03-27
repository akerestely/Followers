#pragma once

#include "CameraSpectator.h"
#include "GLSLProgram.h"

namespace Engine
{
	class Sun
	{
	public:
		Sun(void);
		~Sun(void);

		void Render(const CameraSpectator &camera);

		//getters and setters
		const glm::vec3 GetPosition() { return position; }
		glm::vec3 position;
	private:
		void buildModel();
		void initShaders();

		unsigned int vboId, iboId;
		GLSLProgram *program;
	};
}

