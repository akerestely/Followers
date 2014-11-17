#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager(void)
	{
	}


	InputManager::~InputManager(void)
	{
	}

	void InputManager::PressKey(unsigned int keyID)
	{
		keyMap[keyID] = true;
	}

	void InputManager::ReleaseKey(unsigned int keyID)
	{
		keyMap[keyID] = false;
	}

	bool InputManager::IsKeyPressed(unsigned int keyID)
	{
		auto it = keyMap.find(keyID);
		if(it != keyMap.end())
			return it->second;
		return false;
	}
}
