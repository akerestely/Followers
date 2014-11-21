#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager(void) : mouseCoords(0), mouseCoordsRel(0)
	{
	}


	InputManager::~InputManager(void)
	{
	}

	void InputManager::Update()
	{
		mouseCoordsRel.x=0;
		mouseCoordsRel.y=0;
	}


	void InputManager::PressKey(unsigned int keyID)
	{
		keyMap[keyID] = true;
	}

	void InputManager::ReleaseKey(unsigned int keyID)
	{
		keyMap[keyID] = false;
	}

	bool InputManager::IsKeyDown(unsigned int keyID)
	{
		auto it = keyMap.find(keyID);
		if(it != keyMap.end())
			return it->second;
		return false;
	}

	void InputManager::SetMouseCoords(int x, int y)
	{
		mouseCoords.x=x;
		mouseCoords.y=y;
	}

	void InputManager::SetMouseCoordsRel(int x, int y)
	{
		mouseCoordsRel.x=x;
		mouseCoordsRel.y=y;
	}

}
