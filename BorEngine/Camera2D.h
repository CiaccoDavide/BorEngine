#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
namespace BorEngine
{
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		glm::vec2 screenToWorldCoords(glm::vec2 screenCoords);

		bool isBoxInView(const glm::vec2& position, const glm::vec2& size);

		void setPosition(const glm::vec2& newPosition)
		{
			_position = newPosition;
			_needsMatrixUpdate = true;
		}
		glm::vec2 getPosition() { return _position; }

		void setScale(float newScale)
		{
			_scale = newScale;
			_needsMatrixUpdate = true;
		}
		float getScale() { return _scale; }

		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

	private:
		int _screenWidth = 240;
		int _screenHeight = 120;
		bool _needsMatrixUpdate = true;
		glm::vec2 _position = glm::vec2(0.0f, 0.0f);
		glm::mat4 _cameraMatrix = glm::mat4(1.0f);
		glm::mat4 _orthoMatrix = glm::mat4(1.0f);
		float _scale = 1.0f;
	};
}
