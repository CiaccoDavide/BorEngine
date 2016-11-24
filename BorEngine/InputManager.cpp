#include "InputManager.h"

namespace BorEngine
{



	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		// it: iterator
		//for (auto it = _keyMap.begin(); it != _keyMap.end(); it++)
		for (auto it : p_keyMap)
		{
			p_prevKeyMap[it.first] = it.second;
		}
	}

	void InputManager::keyDown(unsigned int keyID)
	{
		p_keyMap[keyID] = true;
	}
	void InputManager::keyUp(unsigned int keyID)
	{
		p_keyMap[keyID] = false;
	}
	void InputManager::setMouseCoords(float x, float y)
	{
		p_mouseCoords.x = x;
		p_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		/*auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}*/



		if (p_keyMap.find(keyID) != p_keyMap.end())
			return p_keyMap[keyID];
		return false;
	}
	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		return  (isKeyDown(keyID) && !p_wasKeyDown(keyID));
	}
	bool InputManager::p_wasKeyDown(unsigned int keyID)
	{
		if (p_prevKeyMap.find(keyID) != p_prevKeyMap.end())
			return p_prevKeyMap[keyID];
		return false;
	}
}