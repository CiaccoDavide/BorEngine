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
			p_position = newPosition;
			p_needsMatrixUpdate = true;
		}
		glm::vec2 getPosition() { return p_position; }

		void setScale(float newScale)
		{
			p_scale = newScale;
			p_needsMatrixUpdate = true;
		}
		float getScale() { return p_scale; }

		glm::mat4 getCameraMatrix() { return p_cameraMatrix; }

	private:
		int p_screenWidth = 1080;
		int p_screenHeight = 640;
		bool p_needsMatrixUpdate = true;
		glm::vec2 p_position = glm::vec2(0.0f, 0.0f);
		glm::mat4 p_cameraMatrix = glm::mat4(1.0f);
		glm::mat4 p_orthoMatrix = glm::mat4(1.0f);
		float p_scale = 1.0f;
	};
}
