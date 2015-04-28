#include "ModelManager.h"
#include <ctime>

ModelManager::ModelManager(const Level *level)
{
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/Grass/grass_01.obj"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/Boy/boy.lwo"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/Farmhouse/Farmhouse OBJ.obj"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/GrassPack/grass01.3ds"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/GrassPack/grass02.3ds"));

	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/GrassPack/grass03.3ds"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/TreePack/tree01.3ds"));
	models.push_back(Engine::ModelLoader::LoadModelAssimp("Resources/Models/TreePack/tree02.3ds"));
	models.push_back(Engine::ModelLoader::LoadAnimModelAssimp("Resources/Models/boblamp/boblampclean.md5mesh"));	


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
				if(rand()%100 > 1)
					continue;
				ModelElement me;
				me.model = models[6 + rand()%2];
				me.position = glm::vec3(j * cosMeridian * CELL_SIZE, level->GetHeight(glm::vec2(j * cosMeridian * CELL_SIZE,i*CELL_SIZE)) , i*CELL_SIZE);
				me.rotationY = rand() % 360;
				modelElements.push_back(me);
			}
		}
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
		if(glm::length(-camera.GetPosition() - it->position) < 700*CELL_SIZE)
		{
			it->model->Position = it->position;
			it->model->RotateY = it->rotationY;
			it->model->Render(camera, sun);
			count++;
		}
	}
	time++;
	//printf("%d models rendered \n", count);
}
