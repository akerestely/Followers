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
}
