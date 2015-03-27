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
		const glm::vec3 &GetPosition() const { return position; }
		const glm::vec3 &GetRotation() const { return rotation; }
		const glm::mat4 &GetCameraMatrix() const { return cameraMatrix; }
		const glm::mat4 &GetViewMatrix() const { return viewMatrix; }
		const glm::mat4 &GetProjectionMatrix() const { return projectionMatrix; }
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

