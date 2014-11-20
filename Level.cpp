#include "Level.h"

#include <fstream>

#include "Engine/Errors.h"

Level::Level(const std::string &fileName) : nCols(0), nRows(0), levelData(nullptr)
{
	std::ifstream file(fileName,std::ios::binary);
	if(file.fail())
		Engine::fatalError("File could not be opened!");
	file.read((char*)&nCols,sizeof(nCols));
	file.read((char*)&nRows,sizeof(nRows));

	levelData = new float[nRows*nCols];
	file.read((char*)levelData,sizeof(float)*nRows*nCols);
}

Level::~Level(void)
{
	delete[] levelData;
}

void Level::readAscFile(const std::string &fileName)
{
	std::ifstream file(fileName);
	if(file.fail())
		Engine::fatalError("File could not be opened!");

	std::string temp; ///temporary string

	file>>temp>>nCols; ///ignore word and read nCols

	file>>temp>>nRows; ///ignore word and read nRows

	for (int i=0; i < 5; i++)
		std::getline(file,temp); ///ignore next 4 rows

	//allocate memory
	levelData = new float[nRows*nCols];
	//read actual matrix data
	for (int y=0; y<nRows; ++y)
	{
		const int FINISHED_ELEMENTS = y*nCols;
		for (int x=0; x<nCols; ++x)
			file>>levelData[FINISHED_ELEMENTS + x];
	}
}

void Level::writeToBinary(const std::string &fileName, unsigned int nCols, unsigned int nRows)
{
	std::ofstream file(fileName,std::ios::binary);
	if(file.fail())
		Engine::fatalError("File could not be opened!");
	if(!nCols) nCols = this->nCols;
	if(!nRows) nRows = this->nRows;

	file.write((char*)&nCols,sizeof(nCols));
	file.write((char*)&nRows,sizeof(nRows));

	for(int i=0; i<nRows; i++)
		file.write((char*)&levelData[i*this->nCols],sizeof(float)*nCols);
}
