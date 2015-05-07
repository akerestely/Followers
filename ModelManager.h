#pragma once
#include "Engine/ModelLoader.h"
#include "Level.h"

class ModelManager
{
public:
	ModelManager(const Level *level);
	~ModelManager(void);

	void Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun);
private:
	struct ModelElement
	{
		Engine::Model *model;
		float rotationY;
		glm::vec3 position;
	};
	std::vector<Engine::Model*> models;
	std::vector<Engine::Model*> particleModels;
	std::vector<ModelElement> modelElements;
};

