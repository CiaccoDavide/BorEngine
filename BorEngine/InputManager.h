#pragma once

#include <unordered_map>
#include <glm\glm.hpp>

namespace BorEngine
{

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void keyDown(unsigned int keyID);
		void keyUp(unsigned int keyID);
		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		void setMouseCoords(float x, float y);

		// mettiamo const perche' questa funzione non cambia nulla dello stato dell'inputManager!
		glm::vec2 getMouseCoords() const { return _mouseCoords; };

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _prevKeyMap;
		glm::vec2 _mouseCoords;
		bool _wasKeyDown(unsigned int keyID);
	};

}