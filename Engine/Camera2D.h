#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	class Camera2D
	{
	public:
		Camera2D(void);
		~Camera2D(void);

		void Init(int screenWidth, int screenHeight);
		void Update();

		glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);

		bool IsBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions);

		//setters
		void SetPosition(const glm::vec2 &newPosition){ position = newPosition; needsUpdate=true; }
		void SetScale(float newScale) { scale = newScale; needsUpdate=true; }

		//getters
		glm::vec2 GetPosition() { return position; }
		float GetScale() { return scale; }
		glm::mat4 GetCameraMatrix() { return cameraMatrix; }
	private:
		int screenWidth, screenHeight;
		bool needsUpdate;
		float scale;
		glm::vec2 position;
		glm::mat4 cameraMatrix;
		glm::mat4 orthoMatrix;
	};
}

