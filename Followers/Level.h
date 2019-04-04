#pragma once
#include <string>
#include <glm/glm.hpp>

const int ROW = 212;
const int COL = 212;
const float CELL_SIZE = 40.0;

class Level
{
public:
	Level(const std::string &fileName);
	~Level(void);

	//getters
	const float* GetData() const { return levelData; }
	const unsigned int GetRows() const { return nRows; }
	const unsigned int GetCols() const { return nCols; }
	const float GetCosMeridian() const { return cosMeridian; }
	const float GetMaxHeight() const { return maxHeight; }
	const float GetMinHeight() const { return minHeight; }
	const float GetHeight(glm::vec2 point2d) const;
private:
	void readBinaryData(const std::string &fileName);
	void readAscFile(const std::string &fileName);
	void writeToBinary(const std::string &fileName,unsigned int nCols=0U, unsigned int nRows=0U);

	void computeMaxMinHeight();	

	unsigned int nCols, nRows;
	float xllcorner, yllcorner, cellsize, noData_value;
	float maxHeight, minHeight, cosMeridian;
	float *levelData;
};

