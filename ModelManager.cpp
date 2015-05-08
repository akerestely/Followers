#include "ModelManager.h"

#include "Engine/ParticleSystem.h"
#include "Engine/Errors.h"

#include <ctime>
#include <fstream>
#include <unordered_map>

struct ModelInfo
{
	unsigned int type;
	std::string path;
};

ModelManager::ModelManager(const Level *level) : crtModelElement(-1)
{
	this->level = level;
	load();
	
	std::vector<Engine::ParticleSystem *> particleSystems;
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/GrassPack/grass01.3ds"));
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/GrassPack/grass02.3ds"));
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/GrassPack/grass03.3ds"));
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/TreePack/tree01.3ds"));
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/TreePack/tree02.3ds"));

	srand(time(0));
	//add grass
	const float *levelData = level->GetData();
	double cosMeridian = level->GetCosMeridian();
	for(int i=1; i<ROW-1; i++)
		for (int j=1; j<COL-1; j++)
		{
			float h = levelData[i*level->GetCols() + j];
			if(h>1400 && h<1700)
			{
				if(rand()%100 > 50)
					continue;
				unsigned int type = rand() % 5;
				glm::vec3 position(j * cosMeridian * CELL_SIZE, level->GetHeight(glm::vec2(j * cosMeridian * CELL_SIZE,i*CELL_SIZE)) , i*CELL_SIZE);
				float rotationY = rand() % 360;
				float scale;
				switch(type)
				{
				case 0:
				case 1:
				case 2:
					scale = 25;
					break;
				case 3:
				case 4:
					scale = 200;
					break;
				}
				particleSystems[type]->Add(position, rotationY, scale);
			}
		}

	for(auto it = particleSystems.begin(); it != particleSystems.end(); it++)
		particleModels.push_back((*it)->Finalize());

	//NewModel();
}

ModelManager::~ModelManager(void)
{
	for(auto it = models.begin(); it < models.end(); it++)
		delete (*it);
}

void ModelManager::Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun)
{
	static float time = 0;
	for (auto it = modelElements.begin(); it<modelElements.end(); it++)
	{
		it->model->Update(time);
		it->model->Position = it->position;
		it->model->RotateY = it->rotationY;
		it->model->Scale = it->scale;
		it->model->Render(camera, sun);
	}
	//render particles
	for(auto it = particleModels.begin(); it != particleModels.end(); it++)
		(*it)->Render(camera, sun);
	time++;
}

void ModelManager::NextModel()
{
	crtSel = (crtSel+1) % models.size();
	modelElements[crtModelElement].model = models[crtSel];
	modelElements[crtModelElement].selection = crtSel;
}

void ModelManager::NewModel()
{
	modelElements.push_back(ModelElement());
	crtModelElement=modelElements.size()-1;

	if (modelElements.size() == 1)
	{
		crtSel = 0;
		modelElements[crtModelElement].model = models[0];
	}
	else
	{
		modelElements[crtModelElement].model = modelElements[crtModelElement-1].model;
		modelElements[crtModelElement].position = modelElements[crtModelElement-1].position;
		modelElements[crtModelElement].rotationY = modelElements[crtModelElement-1].rotationY;
		modelElements[crtModelElement].scale = modelElements[crtModelElement-1].scale;
		modelElements[crtModelElement].selection = modelElements[crtModelElement-1].selection;
	}
}

void ModelManager::PreviousSelection()
{
	crtModelElement--;
	crtSel = modelElements[crtModelElement].selection;
}

void ModelManager::Rotate(float deltaDegree)
{
	modelElements[crtModelElement].rotationY += deltaDegree;
}

void ModelManager::Scale(float deltaScale)
{
	modelElements[crtModelElement].scale += deltaScale;
	if(modelElements[crtModelElement].scale <= 1.0)
		modelElements[crtModelElement].scale = 1.0;
}

void ModelManager::Position(float x, float z)
{
	 Position(x, level->GetHeight(glm::vec2(x,z)), z);
}

void ModelManager::Position(float x, float y, float z)
{
	modelElements[crtModelElement].position = glm::vec3(x, y, z);
}

void ModelManager::Height(float deltaHeight)
{
	modelElements[crtModelElement].position.y += deltaHeight;
}

void ModelManager::Save()
{
	std::ofstream file("Resources/Map/modelElements",std::ios::binary);
	if(file.fail())
		Engine::fatalError("File could not be opened!");

	unsigned int nElements = modelElements.size();
	file.write((char*)&nElements, sizeof(nElements));
	file.write((char*)&modelElements[0], sizeof(modelElements[0])*nElements);
}

void ModelManager::load()
{
	std::ifstream file("Resources/Map/models");
	if(file.fail())
		Engine::fatalError("File could not be opened!");

	std::unordered_map<unsigned int, ModelInfo> modelPaths;

	unsigned int id;
	ModelInfo mi;

	while (!file.eof())
	{
		file>>id>>mi.type;
		std::getline(file, mi.path);
		//remove first character
		mi.path.erase(0,1);
		modelPaths[id] = mi;
	}

	models.resize(modelPaths.size());
	for(auto it = modelPaths.begin(); it != modelPaths.end(); it++)
		if (it->second.type == 1)
			models[it->first] = Engine::ModelLoader::LoadModelAssimp(it->second.path.c_str());
		else if (it->second.type == 2)
			models[it->first] = Engine::ModelLoader::LoadAnimModelAssimp(it->second.path.c_str());
	file.close();

	//load saved Model Elements
	file.open("Resources/Map/modelElements",std::ios::binary);
	if(file.fail())
		return;
	unsigned int nElements;
	file.read((char*)&nElements,sizeof(nElements));
	
	modelElements.resize(nElements);
	file.read((char*)&modelElements[0], sizeof(modelElements[0])*nElements);

	crtModelElement = nElements-1;

	for (auto it=modelElements.begin(); it != modelElements.end(); it++)
		it->model = models[it->selection];

	crtSel = modelElements[crtModelElement].selection;
}
