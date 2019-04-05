#include "ModelManager.h"

#include "Engine/ParticleSystem.h"
#include "Engine/Errors.h"

#include <ctime>
#include <fstream>

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
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/Rocks/rock1.dae"));
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/Rocks/rock2.dae"));
	particleSystems.push_back(new Engine::ParticleSystem("Resources/Models/Rocks/rock3.dae"));

	srand(time(0));
	//add grass
	const float *levelData = level->GetData();
	double cosMeridian = level->GetCosMeridian();
	for(int i=1; i<ROW-1; i++)
		for (int j=1; j<COL-1; j++)
		{
			float h = levelData[i*level->GetCols() + j];
			if(h>1700)
			{
				if(rand()%500 > 0)
					continue;
				unsigned int type = 5 + rand() % 3;
				glm::vec3 position(j * cosMeridian * CELL_SIZE, level->GetHeight(glm::vec2(j * cosMeridian * CELL_SIZE,i*CELL_SIZE)) , i*CELL_SIZE);
				float rotationY = rand() % 360;
				float scale = 30;
				particleSystems[type]->Add(position, rotationY, scale);
			}
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
	for(auto itMap : models)
		delete itMap.second;
}

void ModelManager::Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun)
{
	static float time = 0;
	for (ModelElement& modelElement : modelElements)
	{
		Engine::Model* model = models[modelElement.modelId];
		model->Update(time);
		model->Position = modelElement.position;
		model->RotateY = modelElement.rotationY;
		model->Scale = modelElement.scale;
		model->Render(camera, sun);
	}
	//render particles
	for(auto it = particleModels.begin(); it != particleModels.end(); it++)
		(*it)->Render(camera, sun);
	time++;
}

void ModelManager::NextModel()
{
	crtSel = (crtSel+1) % models.size();
	modelElements[crtModelElement].modelId = crtSel;
}

void ModelManager::NewModel()
{
	modelElements.push_back(ModelElement());
	crtModelElement=modelElements.size()-1;

	if (modelElements.size() == 1)
	{
		crtSel = 0;
		modelElements[crtModelElement].modelId = models.begin()->first;
	}
	else
	{
		modelElements[crtModelElement].position = modelElements[crtModelElement-1].position;
		modelElements[crtModelElement].rotationY = modelElements[crtModelElement-1].rotationY;
		modelElements[crtModelElement].scale = modelElements[crtModelElement-1].scale;
		modelElements[crtModelElement].modelId = modelElements[crtModelElement-1].modelId;
	}
}

void ModelManager::PreviousSelection()
{
	crtModelElement--;
	crtSel = modelElements[crtModelElement].modelId;
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
		if(mi.type == 1)
			models[id] = Engine::ModelLoader::LoadModelAssimp(mi.path.c_str());
		else if (mi.type == 2)
			models[id] = Engine::ModelLoader::LoadAnimModelAssimp(mi.path.c_str());
	}
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

	crtSel = modelElements[crtModelElement].modelId;
}
