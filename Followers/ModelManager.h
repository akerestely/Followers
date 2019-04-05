#pragma once
#include "Engine/ModelLoader.h"
#include "Level.h"

#include <unordered_map>

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
		glm::vec3 position;
		float rotationY;
		float scale;
		unsigned int modelId;

		ModelElement() : rotationY(0.0), scale(1.0), modelId(0)
		{
		}
	};

	void load();

	std::unordered_map<unsigned int, Engine::Model*> models;
	std::vector<Engine::Model*> particleModels;
	std::vector<ModelElement> modelElements;

	const Level *level;

	//current selection related
	unsigned int crtSel;
	unsigned int crtModelElement;
};