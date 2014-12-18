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
		keyMap[keyID].pressed = true;
	}

	void InputManager::ReleaseKey(unsigned int keyID)
	{
		keyMap[keyID].pressed = false;
		keyMap[keyID].usedOnce = false;
	}

	bool InputManager::IsKeyDown(unsigned int keyID)
	{
		auto it = keyMap.find(keyID);
		if(it != keyMap.end())
			return it->second.pressed;
		return false;
	}


	bool InputManager::IsKeyDownOnce(unsigned int keyID)
	{
		auto it = keyMap.find(keyID);
		if(it != keyMap.end())
			if(it->second.pressed && !it->second.usedOnce)
			{
				it->second.usedOnce = true;
				return true;
			}
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
