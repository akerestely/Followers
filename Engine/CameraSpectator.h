#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class CameraSpectator
	{
	public:
		CameraSpectator(void);
		~CameraSpectator(void);

		void Init(const int screenWidth, const int screeHeight );
		void Update();

		void Move(const glm::vec3 &pos);
		void Rotate(const glm::vec3 &rot) { rotation += rot; needsUpdate = true;}

		//Getters
		const glm::mat4 &GetCameraMatrix() { return cameraMatrix; }
		const glm::mat4 &GetViewMatrix() { return viewMatrix; }
		const glm::mat4 &GetProjectionMatrix() { return projectionMatrix; }
	private:
		int screenWidth, screeHeight;

		bool needsUpdate;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 cameraMatrix;
	};
}

