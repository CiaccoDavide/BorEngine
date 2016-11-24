#include "Camera2D.h"
#include <string>
#include <iostream>

namespace BorEngine
{
	Camera2D::Camera2D()
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		p_screenWidth = screenWidth;
		p_screenHeight = screenHeight;
		p_orthoMatrix = glm::ortho(0.0f, (float)p_screenWidth, 0.0f, (float)p_screenHeight);
	}

	void Camera2D::update()
	{
		if (p_needsMatrixUpdate)
		{
			glm::vec3 translate(-p_position.x + p_screenWidth / 2, -p_position.y + p_screenHeight / 2, 0.0f);
			p_cameraMatrix = glm::translate(p_orthoMatrix, translate);

			glm::vec3 scale(p_scale, p_scale, 1.0f);
			p_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * p_cameraMatrix;

			p_needsMatrixUpdate = false;
			//std::cout << "\n [  CAM  ] pos: " << _position.x << ", " << _position.y << " scale:" << _scale;
		}
	}

	glm::vec2 Camera2D::screenToWorldCoords(glm::vec2 screenCoords)
	{
		screenCoords.y = p_screenHeight - screenCoords.y;
		screenCoords -= glm::vec2(p_screenWidth / 2, p_screenHeight / 2);
		screenCoords /= p_scale;
		screenCoords += p_position;
		return screenCoords;
	}

	/*bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 distanceFromCamera;

		distanceFromCamera.x = abs(abs(_position.x) - abs(position.x)) - size.x;
		distanceFromCamera.y = abs(abs(_position.y) - abs(position.y)) - size.y;
		if (abs(distanceFromCamera.x) > _screenWidth / 2 || abs(distanceFromCamera.y) > _screenHeight / 2)
			return false;

		return true;
	}*/
	// Simple AABB test to see if a box is in the camera view
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) {

		glm::vec2 scaledScreenDimensions = glm::vec2(p_screenWidth, p_screenHeight) / (p_scale);

		// The minimum distance before a collision occurs
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		// Center position of the parameters
		glm::vec2 centerPos = position + dimensions / 2.0f;
		// Center position of the camera
		glm::vec2 centerCameraPos = p_position;
		// Vector from the input to the camera
		glm::vec2 distVec = centerPos - centerCameraPos;

		// Get the depth of the collision
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		// If both the depths are > 0, then we collided
		if (xDepth > 0 && yDepth > 0) {
			// There was a collision
			return true;
		}
		return false;
	}
}