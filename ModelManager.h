#pragma once
#include "Engine/ModelLoader.h"
#include "Level.h"

class ModelManager
{
public:
	ModelManager(const Level *level);
	~ModelManager(void);

	void Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun);

 	void NextModel();
	void NewModel();
	void PreviousSelection();

 	void Rotate(float deltaDegree);
 	void Scale(float deltaScale);
	void Position(float x, float z);
	void Position(float x, float y, float z);
	void Height(float deltaHeight);

	void Save();
private:
	struct ModelElement
	{
		Engine::Model *model;
		float rotationY;
		glm::vec3 position;
		float scale;
		unsigned int selection;

		ModelElement()
		{
			model = nullptr;
			rotationY = 0.0;
			scale = 1.0;
			selection = 0;
		}
	};

	void load();

	std::vector<Engine::Model*> models;
	std::vector<Engine::Model*> particleModels;
	std::vector<ModelElement> modelElements;

	const Level *level;

	//current selection related
	unsigned int crtSel;
	unsigned int crtModelElement;
};