#include "InputManager.h"

namespace BorEngine
{



	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		// it: iterator
		//for (auto it = _keyMap.begin(); it != _keyMap.end(); it++)
		for (auto it : _keyMap)
		{
			_prevKeyMap[it.first] = it.second;
		}
	}

	void InputManager::keyDown(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}
	void InputManager::keyUp(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}
	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
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
	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		return  (isKeyDown(keyID) && !_wasKeyDown(keyID));
	}
	bool InputManager::_wasKeyDown(unsigned int keyID)
	{
		if (_prevKeyMap.find(keyID) != _prevKeyMap.end())
			return _prevKeyMap[keyID];
		return false;
	}
}