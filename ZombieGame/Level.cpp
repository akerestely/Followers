#include "Level.h"

#include <fstream>

#include <Engine/Errors.h>
#include "Engine/ResourceMngr.h"

Level::Level(const std::string &fileName)
{
	std::ifstream file;
	file.open(fileName);

	if(file.fail())
		Engine::fatalError("Failed to open level file!");
	
	std::string tmp;
	file>>tmp>>numHumans;

	while(std::getline(file,tmp))
	{
		levelData.push_back(tmp);
	}
	
	spriteBatch.Init();
	spriteBatch.Begin();

	glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);

	for(int i=0; i<levelData.size();i++)
		for (int j=0; j<levelData[i].size(); j++)
		{
			char tile = levelData[i][j];

			glm::vec4 destRect(j*TILE_WIDTH, i*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile)
			{
			case 'R':
				spriteBatch.Draw(destRect,uvRect,Engine::ResourceMngr::GetTexture("Textures/red_bricks.png").id,0.0f,Engine::Color(255,255,255,255));
				break;
			case 'B':
				spriteBatch.Draw(destRect,uvRect,Engine::ResourceMngr::GetTexture("Textures/red_bricks.png").id,0.0f,Engine::Color(255,255,255,255));
				break;
			case 'G':
				spriteBatch.Draw(destRect,uvRect,Engine::ResourceMngr::GetTexture("Textures/glass.png").id,0.0f,Engine::Color(255,255,255,255));
				break;
			case 'L':
				spriteBatch.Draw(destRect,uvRect,Engine::ResourceMngr::GetTexture("Textures/light_bricks.png").id,0.0f,Engine::Color(255,255,255,255));
				break;
			case '@':
				playerStartPos.x = j*TILE_WIDTH;
				playerStartPos.y = i*TILE_WIDTH;
				break;
			case 'Z':
				zombieStartPositions.push_back(glm::ivec2(j*TILE_WIDTH,i*TILE_WIDTH));
				break;
			case '.':
				break;
			default:
				printf("Unexpected symbol %c at (%d,%d)", tile, j,i);
			}
		}
	spriteBatch.End();
}


Level::~Level(void)
{
}

void Level::Draw()
{
	spriteBatch.RenderBatches();
}
