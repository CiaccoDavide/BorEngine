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
		glm::vec2 getMouseCoords() const { return p_mouseCoords; };

	private:
		std::unordered_map<unsigned int, bool> p_keyMap;
		std::unordered_map<unsigned int, bool> p_prevKeyMap;
		glm::vec2 p_mouseCoords = glm::vec2(0.0f);
		bool p_wasKeyDown(unsigned int keyID);
	};

}