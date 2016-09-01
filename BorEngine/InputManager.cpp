#include "InputManager.h"

namespace BorEngine
{



	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::keyDown(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}
	void InputManager::keyUp(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}
	bool InputManager::isKeyDown(unsigned int keyID)
	{
		/*auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}*/



		if (_keyMap.find(keyID) != _keyMap.end())
			return _keyMap[keyID];
		return false;
	}
}