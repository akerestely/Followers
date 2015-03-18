#include "CameraSpectator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

const  glm::vec3 OX(1.0f, 0.0f, 0.0f);
const  glm::vec3 OY(0.0f, 1.0f, 0.0f);
const float piDiv180 = 0.01745329251994f;

namespace Engine
{
	CameraSpectator::CameraSpectator(void) 
	:	screenWidth(500), 
		screeHeight(500), 
		position(0.0f), 
		rotation(0.0f),
		viewMatrix(1.0f),
		projectionMatrix(1.0f), 
		cameraMatrix(1.0f),
		needsUpdate(true)
	{
		//empty
	}


	CameraSpectator::~CameraSpectator(void)
	{
		//empty
	}

	void CameraSpectator::Init(const int screenWidth, const int screeHeight)
	{
		this->screenWidth = screenWidth;
		this->screeHeight = screeHeight;
		projectionMatrix = glm::perspective(45.0f, (float)screenWidth/screeHeight, 0.125f, 10000.0f);
	}

	void CameraSpectator::Update()
	{
		if(needsUpdate)
		{
			viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));//dist from player
			viewMatrix = glm::rotate(viewMatrix, rotation.x, OX);
			viewMatrix = glm::rotate(viewMatrix, rotation.y, OY);
			viewMatrix = glm::translate(viewMatrix, position);
			cameraMatrix = projectionMatrix * viewMatrix;
			needsUpdate = false;
		}
	}

	void CameraSpectator::Move(const glm::vec3 &pos) 
	{
		//compute direction vector
		//rotate around y axis
		glm::vec3 step1(sin(rotation.y * piDiv180), 0.0f, -cos(rotation.y * piDiv180));
		//rotate around x axis
		float cosX = cos(rotation.x * piDiv180);
		glm::vec3 step2(step1.x *cosX, -sin(rotation.x * piDiv180), step1.z * cosX);

		glm::vec3 viewDir = step2;
		//move on z
		position += viewDir * pos.z;
		//move on x
		position.x += viewDir.z * pos.x;
		position.z += -viewDir.x * pos.x;
		//move on y
		position.y -= pos.y;
		needsUpdate = true;
	}
}
