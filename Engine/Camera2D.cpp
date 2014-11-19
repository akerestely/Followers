#include "Camera2D.h"

namespace Engine
{

	Camera2D::Camera2D(void):
		position(0.0f, 0.0f),
		cameraMatrix(1.0f),
		orthoMatrix(1.0f),
		scale(1.0f),
		needsUpdate(true),
		screenWidth(500),
		screenHeight(500)
		{
		}

	Camera2D::~Camera2D(void)
	{
	}

	void Camera2D::Init(int screenWidth, int screenHeight)
	{
		this->screenWidth=screenWidth;
		this->screenHeight=screenHeight;
		orthoMatrix = glm::ortho(0.0f, (float)this->screenWidth, 0.0f, (float)this->screenHeight);
	}

	void Camera2D::Update()
	{
		if(needsUpdate)
		{
			glm::vec3 translate(-position.x + screenWidth/2, -position.y + screenHeight/2, 0.0f);
			cameraMatrix = glm::translate(orthoMatrix, translate);
			glm::vec3 scale (scale, scale, 0.0f);
			cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * cameraMatrix;
			needsUpdate = false;
		}
	}

	glm::vec2 Camera2D::ConvertScreenToWorld(glm::vec2 screenCoords)
	{
		screenCoords.y = screenHeight - screenCoords.y;
		screenCoords -= glm::vec2(screenWidth/2, screenHeight/2);
		screenCoords /= scale;
		screenCoords += position;
		return screenCoords;
	}

	bool Camera2D::IsBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions)
	{
		glm::vec2 scaledScreenDimensions = glm::vec2(screenWidth,screenHeight) / scale;

		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

		glm::vec2 centerPos = position + dimensions / 2.0f;
		glm::vec2 distVec = centerPos - this->position;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if(xDepth > 0 && yDepth > 0)
		{
			return true;
		}
		return false;
	}

}
