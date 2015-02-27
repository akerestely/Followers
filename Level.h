#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Engine/Vertex.h"

class Level
{
public:
	Level(const std::string &fileName);

	void SwitchWireframeVisibility();

	void Render();

	double GetHeight(glm::vec2 point2d);
	void GetMaxMinHeight(float &maxHeight, float &minHeight);

	~Level(void);
private:
	void readBinaryData(const std::string &fileName);
	void readAscFile(const std::string &fileName);
	void writeToBinary(const std::string &fileName,unsigned int nCols=0U, unsigned int nRows=0U);

	

	Engine::ColorRGBA8 getColorByHeight(float height);
	Engine::Position Level::normal(Engine::Position p1, Engine::Position p2, Engine::Position p3);

	unsigned int nCols, nRows;
	float xllcorner, yllcorner, cellsize, noData_value;
	float *levelData;

	unsigned int vboId,iboId;
	unsigned int vboIdWireframe,iboIdWireframe;
	unsigned int vboNormals, iboNormals;
	bool showWireframe;

	std::map<float,Engine::ColorRGBA8> heightColorMap;
};

