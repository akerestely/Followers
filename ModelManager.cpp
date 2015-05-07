#include "ModelManager.h"

#include "Engine/ParticleSystem.h"

#include <ctime>

ModelManager::ModelManager(const Level *level)
{
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/Grass/grass_01.obj"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/Boy/boy.lwo"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/Farmhouse/Farmhouse OBJ.obj"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/GrassPack/grass01.3ds"));
	models[models.size()-1]->Scale = 50;
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/GrassPack/grass02.3ds"));
	models[models.size()-1]->Scale = 50;

	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/GrassPack/grass03.3ds"));
	models[models.size()-1]->Scale = 50;
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/TreePack/tree01.3ds"));
	models[models.size()-1]->Scale = 300;
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/TreePack/tree02.3ds"));
	models[models.size()-1]->Scale = 300;
	//models.push_back(Engine::ModelLoader::LoadAnimModelAssimp("Resources/Models/boblamp/boblampclean.md5mesh"));
	models.push_back(Engine::ModelLoader::LoadAnimModelAssimp("Resources/Models/windmill.dae"));
	models[models.size()-1]->Scale = 50;

	
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
// 				ModelElement me;
// 				me.model = models[3 + rand()%1];
// 				me.position = glm::vec3(j * cosMeridian * CELL_SIZE, level->GetHeight(glm::vec2(j * cosMeridian * CELL_SIZE,i*CELL_SIZE)) , i*CELL_SIZE);
// 				me.rotationY = rand() % 360;
// 				modelElements.push_back(me);
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

	ModelElement me;
	me.model = models[8];
	float x = 0 * cosMeridian * CELL_SIZE;
	//float h = levelData[100*level->GetCols() + 50];
	float z = 0*CELL_SIZE;
	me.position = glm::vec3(x, level->GetHeight(glm::vec2(x,z)), z);
	//me.rotationY = rand() % 360;
	modelElements.push_back(me);
	printf("Number of models in memory: %d \n", modelElements.size());
}

ModelManager::~ModelManager(void)
{
	for(auto it = models.begin(); it < models.end(); it++)
		delete (*it);
}

void ModelManager::Render(const Engine::CameraSpectator &camera, const Engine::Sun *sun)
{
	static float time = 0;
	int count = 0;
	models[8]->Update(time);
	for (auto it = modelElements.begin(); it<modelElements.end(); it++)
	{
		if(glm::length(-camera.GetPosition() - it->position) < 100*CELL_SIZE)
		{
			it->model->Position = it->position;
			it->model->RotateY = it->rotationY;
			it->model->Render(camera, sun);
			count++;
		}
	}
	for(auto it = particleModels.begin(); it != particleModels.end(); it++)
		(*it)->Render(camera, sun);
	time++;
	//printf("%d models rendered \n", count);
}
