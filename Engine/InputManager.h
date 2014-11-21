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

		void Update();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		bool IsKeyDown(unsigned int keyID);

		void SetMouseCoords(int x, int y);
		void SetMouseCoordsRel(int x, int y);

		glm::ivec2 GetMouseCoords() const { return mouseCoords; }
		glm::ivec2 GetMouseCoordsRel() const { return mouseCoordsRel; }
	private:
		std::unordered_map<unsigned int,bool> keyMap;
		glm::ivec2 mouseCoords;
		glm::ivec2 mouseCoordsRel;
	};
}