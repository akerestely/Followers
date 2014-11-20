#pragma once
#include <string>

class Level
{
public:
	Level(const std::string &fileName);
	~Level(void);
private:
	void readAscFile(const std::string &fileName);
	void writeToBinary(const std::string &fileName,unsigned int nCols=0U, unsigned int nRows=0U);
	unsigned int nCols, nRows;
	float *levelData;
};

