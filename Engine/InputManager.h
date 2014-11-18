#pragma once
#include <unordered_map>
#include <glm\glm.hpp>
namespace Engine
{
	class InputManager
	{
	public:
		InputManager(void);
		~InputManager(void);

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		bool IsKeyPressed(unsigned int keyID);

		void SetMouseCoords(float x, float y);
		glm::vec2 GetMouseCoords() const { return mouseCoords; }
	private:
		std::unordered_map<unsigned int,bool> keyMap;
		glm::vec2 mouseCoords;
	};
}