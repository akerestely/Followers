#pragma once

#include "CameraSpectator.h"
#include "GLSLProgram.h"

namespace Engine
{
	class Sun
	{
	public:
		Sun(int screenWidth, int screenHeight);
		~Sun(void);

		void Update(const CameraSpectator &camera);
		void Render(const CameraSpectator &camera);

		void Resize(int screenWidth, int screenHeight);

		//getters and setters
		const glm::vec3 GetSunPosition() { return sunPosition; }
		const glm::vec3 GetPosition() { return position; }
	private:
		void buildModel();
		void buildRect();
		void initShaders();

		void renderSun(const CameraSpectator &camera);
		void renderRect();

		glm::vec3 sunPosition;
		glm::vec3 position;

		unsigned int vboId, iboId;
		unsigned int vboIdRect, iboIdRect;
		unsigned int fboId;
		//texIdDepth[0] - depth
		//texIdDepth[1] - sun depth
		unsigned int texIdDepth[2];
		//texIdColor[0] - sun color
		//texIdColor[1] - sun color after depth test
		unsigned int texIdColor[2];
		GLSLProgram *program, *depthTestProgram, *radialBlurProgram;
	};
}

