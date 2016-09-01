#pragma once

#include <unordered_map>

namespace BorEngine 
{

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void keyDown(unsigned int keyID);
		void keyUp(unsigned int keyID);
		bool isKeyDown(unsigned int keyID);
	
	private:
		std::unordered_map<unsigned int, bool> _keyMap;
	};
	
}